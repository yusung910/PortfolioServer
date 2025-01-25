#include "stdafx.hxx"
#include "NetworkWorkerPO.hxx"

#include "NetworkManager.h"
#include "NetworkHostPO.hxx"
#include "NetworkContextPO.hxx"

#include <process.h>
#include <string>

NetworkWorkerPO::NetworkWorkerPO()
    :m_bIsTerminated(false)
    , m_hIOCP(INVALID_HANDLE_VALUE)
{
}

NetworkWorkerPO::~NetworkWorkerPO()
{
    TerminateThread();
    for (auto& handle : m_oHandleList)
    {
        WaitForSingleObject(handle, INFINITE);
    }

    CloseHandle(m_hIOCP);
    m_oHandleList.clear();
}

bool NetworkWorkerPO::CreateThread()
{
    VIEW_WRITE_INFO(L"NetworkWorkerPO::CreateThread() - Begin");

    // IOCP 핸들 생성
    // CreateIoCompletionPort() : IOCP 커널 객체를 생성하거나 IOCP와 디바이스를 연결하는 작업을 진행. 
    // https://learn.microsoft.com/ko-kr/windows/win32/fileio/createiocompletionport
    auto lIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    if (lIOCP == NULL || lIOCP == INVALID_HANDLE_VALUE)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::CreateThread() CreateIoCompletionPort() Failed:%d", GetLastError());

        return false;
    }

    m_hIOCP = lIOCP;

    //스레드 생성
    SYSTEM_INFO lInfo;
    GetSystemInfo(&lInfo);

    for (unsigned int n = 0; n < lInfo.dwNumberOfProcessors; ++n)
    {
        auto lHandle = (HANDLE)_beginthreadex(nullptr, 0, ExecuteThread, this, 0, nullptr);

        if (lHandle == nullptr || lHandle == INVALID_HANDLE_VALUE)
        {
            TerminateThread();
            return false;
        }

        m_oHandleList.push_back(lHandle);
    }

    Sleep(100);


    //
    if (true == m_bIsTerminated)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::CreateThread() Failed! m_bIsTerminated is True");
        return false;
    }

    m_bIsStarted = true;

    VIEW_WRITE_INFO(L"NetworkWorkerPO::CreateThread() - Thread Create Success");

    return true;
}

void NetworkWorkerPO::TerminateThread()
{
    m_bIsStarted = false;
    m_bIsTerminated = true;

    //PostQueuedCompletionStatus() 인자값으로 전달 받은 값을 패킷으로 전송
    //https://learn.microsoft.com/ko-kr/windows/win32/fileio/postqueuedcompletionstatus
    PostQueuedCompletionStatus(m_hIOCP, 0, (ULONG_PTR)nullptr, nullptr);
}

unsigned int WINAPI NetworkWorkerPO::ExecuteThread(void* _arg)
{
    auto& lParent = *(NetworkWorkerPO*)_arg;
    lParent.ProcessThread();
    lParent.TerminateThread();

    return 0;
}

bool NetworkWorkerPO::PushThread(NetworkHostPO* _host, NetworkContextPO* _ctxt)
{
    if (_host == nullptr || _ctxt == nullptr)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::PushThread() Failed - Paramter is nullptr");
        return false;
    }


    if (false == m_bIsStarted)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::PushThread() Failed - IOCP Thread is not Created.");
        return false;
    }

    //인자값으로 전달한 _host, _ctxt에 저장된 값을 패킷으로 전송한다
    if (PostQueuedCompletionStatus(m_hIOCP, 0, (ULONG_PTR)_host, _ctxt) == FALSE)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::PushThread() Failed - PostQueuedCompletionStatus:%d", GetLastError());
        return false;
    }
    return true;
}

bool NetworkWorkerPO::RegisterThread(NetworkHostPO* _host)
{
    if (_host == nullptr)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::RegisterThread() Failed - Parameter is nullptr");
        return false;
    }

    auto lSocket = _host->GetSocket();
    if (lSocket == INVALID_SOCKET)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::RegisterThread() Failed - Socket is null");
        return false;
    }

    // IOCP 핸들 생성
    // CreateIoCompletionPort() : IOCP 커널 객체를 생성하거나 IOCP와 디바이스를 연결하는 작업을 진행. 
    // https://learn.microsoft.com/ko-kr/windows/win32/fileio/createiocompletionport
    if (CreateIoCompletionPort((HANDLE)lSocket, m_hIOCP, (ULONG_PTR)_host, 0) == nullptr)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::RegisterThread() Failed - CreateIoCompletionPort: %d", GetLastError());
        return false;
    }

    return true;
}

void NetworkWorkerPO::ProcessThread()
{
    while (false == m_bIsTerminated)
    {
        bool lRslt = true;
        DWORD lTransferred = 0;
        ULONG_PTR lKey = 0;
        LPOVERLAPPED lOverlapped = 0;

        DWORD lDwLastError = 0;
        // GetQueuedCompletionStatus() : 
        // 이 함수를 호출하면 호출한 스레드는 I/O completion queue에서 데이터를 가져온다 
        // 만약 데이터가 없을 경우 데이터가 전달 될 때 까지 대기.
        //BOOL GetQueuedCompletionStatus(
        //HANDLE       CompletionPort,
        //    LPDWORD      lpNumberOfBytesTransferred,
        //    PULONG_PTR   lpCompletionKey,
        //    LPOVERLAPPED* lpOverlapped,
        //    DWORD        dwMilliseconds
        // )
        //  CompletionPort : 대기를 수행할 대상 IOCP 핸들을 입력
        //  lpNumberOfBytesTransferred : 송수신된 바이트 수(output)
        //  lpCompletionKey	: 비동기 I / O 요청이 발생한 디바이스의 completionKey(output)
        //  lpOverlapped : 비동기 호출시 전달한 Overlapped 구조체의 주소(output)
        //  dwMilliseconds : 대기를 수행할 시간(밀리초)을 입력합니다.
        //    return = TRUE 성공, FALSE 실패
        // https://jungwoong.tistory.com/43
        if (GetQueuedCompletionStatus(m_hIOCP, &lTransferred, &lKey, &lOverlapped, INFINITE) == TRUE)
        {
            if (lKey == 0
                && lTransferred == 0
                && lOverlapped == 0)
                break;

        }
        else
        {
            lDwLastError = GetLastError();
            // ERROR_NETNAME_DELETED: 클라이언트가 HardClose한 경우 발생
            // 일종의 강제 종료
            // https://goguri.tistory.com/749
            if (lDwLastError != ERROR_NETNAME_DELETED)
                lRslt = false;
        }

        auto lHost = (NetworkHostPO*)lKey;
        auto lCtxt = (NetworkContextPO*)lOverlapped;

        if (lHost == nullptr
            || lCtxt == nullptr)
        {
            VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessThread() Failed - Parameter Error");

            continue;
        }

        switch (lCtxt->GetContextType())
        {
        case EContextType::Accept:
            ProcessAccept(*lHost, *lCtxt, lRslt);
            break;
        case EContextType::Connect:
            ProcessConnect(*lHost, *lCtxt, lRslt);
            break;
        case EContextType::Receive:
            ProcessReceive(*lHost, *lCtxt, lRslt, (int)lTransferred);
            break;
        case EContextType::Encrypt:
            ProcessEncrypt(*lHost, *lCtxt);
            break;
        case EContextType::Send:
            ProcessSend(*lHost, *lCtxt, lRslt, (int)lTransferred);
            break;
        default:
            VIEW_WRITE_ERROR("NetworkWorkerPO::ProcessThread() Failed - ContextType :%d", lCtxt->GetContextType());
            break;
        }

        // Error Exception 처리
        if (false == lRslt)
        {
            bool lCustom = false;
            switch (lCtxt->GetContextType())
            {
            case EContextType::Connect:
            {
                lCustom = true;
                switch (lDwLastError)
                {
                case ERROR_CONNECTION_REFUSED:
                case ERROR_SEM_TIMEOUT:
                {
                    VIEW_WRITE_ERROR(L"Connect to [%S:%d] Failed (%d) (Code : %d [%x])", lHost->GetIP().c_str(), lHost->GetPeerPort(), lHost->GetHostID(), lDwLastError, lDwLastError);
                }
                break;

                default:
                    break;
                }
            }
            break;
            case EContextType::Receive:
            {
                if (lDwLastError == ERROR_GRACEFUL_DISCONNECT)
                {
                    lCustom = true;
                    VIEW_WRITE_ERROR(L"Host Gracefull Disconnected (%d), [%S:%d] ", lHost->GetHostID(), lHost->GetIP().c_str(), lHost->GetPeerPort());
                }
            }
            break;
            default:
                break;
            }

            if (false == lCustom)
            {
                switch (lDwLastError)
                {
                case ERROR_CONNECTION_ABORTED: // 접속 끊어짐. Socket Clost 메세지 참조
                    break;
                default:
                    VIEW_WRITE_ERROR(L"Host(%d) Result Failed (Code : %d [%x]) [%S]", lHost->GetHostID(), lDwLastError, lDwLastError, lHost->GetIP().c_str());
                    break;
                }
            }
        }

        NetworkManager::GetInst().ReleaseContext(lCtxt);
    }
}

void NetworkWorkerPO::ProcessAccept(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt)
{
    //Accept 완료 처리
    SOCKET lSocket = INVALID_SOCKET;
    _ctxt.Read(&lSocket, sizeof(lSocket));

    if (_rslt == true)
    {
        //Accept 성공 시 주소정보사이즈(sizeof(SOCKADDR_IN) + 16 * 2)를 늘린다
        _ctxt.Write((sizeof(SOCKADDR_IN) + 16 * 2));

        //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-setsockopt
        //소켓의 옵션을 SO_UPDATE_ACCEPT_CONTEXT 값으로 세팅한다 
        SOCKET lListener = _host.GetSocket();
        if (setsockopt(lSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&lListener, sizeof(lListener)) == SOCKET_ERROR)
        {
            VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessAccept() Failed - SO_UPDATE_ACCEPT_CONTEXT: %d", WSAGetLastError());
        }

        //접속 주소 생성
        std::string lIP = "";
        int lPort = 0;
        int lIPAddr = 0;
        if (nullptr != _ctxt.m_pRemoteAddr)
        {
            lIPAddr = _ctxt.m_pRemoteAddr->sin_addr.S_un.S_addr;
            char tmpIPAddr[INET_ADDRSTRLEN] = { 0, };
            inet_ntop(AF_INET, &_ctxt.m_pRemoteAddr->sin_addr, tmpIPAddr, sizeof(tmpIPAddr));

            lIP = tmpIPAddr;
            lPort = (int)ntohs(_ctxt.m_pRemoteAddr->sin_port);
        }

        NetworkManager::GetInst().Join(_host.GetEvnetSync(), lIPAddr, lIP, lPort, lSocket);

    }
    else
    {
        closesocket(lSocket);
    }

    //NetworkContextPO 초기화
    _ctxt.ResetBuffer();

    //다시 Accept 요청
    if (_host.Accept(_ctxt) == false)
    {
        //Listen Socket을 종료시키지 않기 위해 Accept는 실패해도 성공으로 처리.
        _host.EndBaseTask(true);
        return;
    }
}

void NetworkWorkerPO::ProcessConnect(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt)
{
    if (_rslt == false)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessConnect() Failed - Parameter Result is false");

        _host.EndBaseTask(false, ESocketCloseType::ConnectFailed);

        return;
    }

    //접속 이벤트 호출
    _host.EventConnect(EHostType::Connector);

    //Receive
    _ctxt.ResetBuffer();

    if (_host.Receive(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessConnect() Failed - Receive");

        _host.EndBaseTask(false);
        return;
    }
}


void NetworkWorkerPO::ProcessReceive(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred)
{
    //요청 결과 체크
    if (_rslt == false || _transferred <= 0)
    {
        //NetworkHostType이 EHostType::Acceptor이 아니고(and)
        //_rslt 값이 false 이며(and)
        //_transferred의 값이 0보다 클(_transferred > 0) 경우(and)
        if (!(_host.GetHostType() == EHostType::Acceptor
            && _rslt == true
            && _transferred == 0))
        {
            if (_rslt != false)
                VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessReceive() Failed - Rslt is False");

        }
        _host.EndBaseTask(false);
        return;
    }

    //Receive 한 크기 추가
    _ctxt.Write(_transferred);

    //복호화 처리
    if (_host.Decrypt(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessReceive() Failed - _host.Decrypt()");
        _host.EndBaseTask(false);
        return;
    }

    if (_host.GetHostType() != EHostType::Connector)
        NetworkManager::GetInst().OnRecv(_transferred);
}

void NetworkWorkerPO::ProcessEncrypt(NetworkHostPO& _host, NetworkContextPO& _ctxt)
{
    //요청 결과 체크
    if (_host.Encrypt(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessEncrypt() Failed - _host.Encrypt()");
        _host.EndSendTask(false);
        return;
    }

    if (_host.Send(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessEncrypt() Failed - _host.Send() is False");

        _host.EndSendTask(false);

        return;
    }
}


void NetworkWorkerPO::ProcessSend(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred)
{
    //요청 결과 체크
#ifdef SEALED_SEND_SUCCESS_TRANSFERRED_0
    if (_rslt == false
        || _transferred <= 0)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessSend() Failed - Result(%s), Transferred(%d), HostID(%d), IP(%s)"
            , _rslt ? L"True" : L"False"
            , _transferred
            , _host.GetHostID()
            , StringUtil::ToWideChar(_host.GetIP()).c_str()
        );
        _host.EndSendTask(false);
        return;
    }
#else
    //Transferred 0일 경우에도 일단 전송
    if (_rslt == false)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessSend() Failed - Result is False");
        _host.EndSendTask(false);
        return;
    }
#endif // SEALED_SEND_SUCCESS_TRANSFERRED_0

    // 전송 여부 체크
    if (_ctxt.GetDataSize() != static_cast<size_t>(_transferred))
    {
        VIEW_WRITE_WARNING(L"NetworkWorkerPO::ProcessSend() - Transferred:%d, %d", _ctxt.GetDataSize(), _transferred);
    }

    if (_host.GetHostType() != EHostType::Connector)
        NetworkManager::GetInst().OnSend(_transferred);

    //Send 완료 처리
    _host.EndSendTask(true);
}
