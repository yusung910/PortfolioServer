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
    auto localIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0,0);

    if (localIOCP == NULL || localIOCP == INVALID_HANDLE_VALUE)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::CreateThread() CreateIoCompletionPort() Failed:%d", GetLastError());

        return false;
    }

    m_hIOCP = localIOCP;

    //스레드 생성
    SYSTEM_INFO localInfo;
    GetSystemInfo(&localInfo);

    for (unsigned int n = 0; n < localInfo.dwNumberOfProcessors; ++n)
    {
        auto localHandle = (HANDLE)_beginthreadex(nullptr, 0, ExecuteThread, this, 0, nullptr);

        if (localHandle == nullptr || localHandle == INVALID_HANDLE_VALUE)
        {
            TerminateThread();
            return false;
        }

        m_oHandleList.push_back(localHandle);
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
    auto& localParent = *(NetworkWorkerPO*)_arg;
    localParent.ProcessThread();
    localParent.TerminateThread();

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
    
    auto localSocket = _host->GetSocket();
    if (localSocket == INVALID_SOCKET)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::RegisterThread() Failed - Socket is null");
        return false;
    }

    // IOCP 핸들 생성
    // CreateIoCompletionPort() : IOCP 커널 객체를 생성하거나 IOCP와 디바이스를 연결하는 작업을 진행. 
    // https://learn.microsoft.com/ko-kr/windows/win32/fileio/createiocompletionport
    if (CreateIoCompletionPort((HANDLE)localSocket, m_hIOCP, (ULONG_PTR)_host, 0) == nullptr)
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
        bool localRslt = true;
        DWORD localTransferred = 0;
        ULONG_PTR localKey = 0;
        LPOVERLAPPED localOverlapped = 0;

        DWORD localDwLastError = 0;
        // GetQueuedCompletionStatus() : 
        // 이 함수를 호출하면 호출한 스레드는 I/O compleion queue에서 데이터를 가져온다 
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
        if (GetQueuedCompletionStatus(m_hIOCP, &localTransferred, &localKey, &localOverlapped, INFINITE) == TRUE)
        {
            if (localKey == 0
                && localTransferred == 0
                && localOverlapped == 0)
                break;
            
        }
        else
        {
            localDwLastError = GetLastError();
            // ERROR_NETNAME_DELETED: 클라이언트가 HardClose한 경우 발생
            // 일종의 강제 종료
            // https://goguri.tistory.com/749
            if (localDwLastError != ERROR_NETNAME_DELETED)
                localRslt = false;
        }

        auto localHost = (NetworkHostPO*)localKey;
        auto localCtxt = (NetworkContextPO*)localOverlapped;

        if (localHost == nullptr
            || localCtxt == nullptr)
        {
            VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessThread() Failed - Parameter Error");

            continue;
        }

        switch (localCtxt->GetContextType())
        {
        case EContextType::Accept:
            ProcessAccept(*localHost, *localCtxt, localRslt);
            break;
        case EContextType::Connect:
            ProcessConnect(*localHost, *localCtxt, localRslt);
            break;
        case EContextType::Receive:
            ProcessReceive(*localHost, *localCtxt, localRslt, (int)localTransferred);
            break;
        case EContextType::Encrypt:
            ProcessEncrypt(*localHost, *localCtxt);
            break;
        case EContextType::Send:
            ProcessSend(*localHost, *localCtxt, localRslt, (int)localTransferred);
            break;
        default:
            VIEW_WRITE_ERROR("NetworkWorkerPO::ProcessThread() Failed - ContextType :%d", localCtxt->GetContextType());
            break;
        }

        // Error Exception 처리
        if (false == localRslt)
        {
            bool localCustom = false;
            switch (localCtxt->GetContextType())
            {
            case EContextType::Connect:
            {
                localCustom = true;
                switch (localDwLastError)
                {
                    case ERROR_CONNECTION_REFUSED:
                    case ERROR_SEM_TIMEOUT:
                    {
                        VIEW_WRITE_ERROR(L"Connect to [%S:%d] Failed (%d) (Code : %d [%x])", localHost->GetIP().c_str(), localHost->GetPeerPort(), localHost->GetHostID(), localDwLastError, localDwLastError);
                    }
                    break;

                    default:
                        break;
                }
            }
            break;
            case EContextType::Receive:
            {
                if (localDwLastError == ERROR_GRACEFUL_DISCONNECT)
                {
                    localCustom = true;
                    VIEW_WRITE_ERROR(L"Host Gracefull Disconnected (%d), [%S:%d] ",  localHost->GetHostID(), localHost->GetIP().c_str(), localHost->GetPeerPort());
                }
            }
            break;
            default:
                break;
            }

            if (false == localCustom)
            {
                switch (localDwLastError)
                {
                case ERROR_CONNECTION_ABORTED: // 접속 끊어짐. Socket Clost 메세지 참조
                    break;
                default:
                    VIEW_WRITE_ERROR(L"Host(%d) Result Failed (Code : %d [%x]) [%S]", localHost->GetHostID(), localDwLastError, localDwLastError, localHost->GetIP().c_str());
                    break;
                }
            }
        }

        NetworkManager::GetInst().ReleaseContext(localCtxt);
    }
}

void NetworkWorkerPO::ProcessAccept(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt)
{
    //Accept 완료 처리
    SOCKET localSocket = INVALID_SOCKET;
    _ctxt.Read(&localSocket, sizeof(localSocket));

    if (_rslt == true)
    {

    }
}

void NetworkWorkerPO::ProcessConnect(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt)
{
}

void NetworkWorkerPO::ProcessReceive(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred)
{
}

void NetworkWorkerPO::ProcessEncrypt(NetworkHostPO& _host, NetworkContextPO& _ctxt)
{
}

void NetworkWorkerPO::ProcessSend(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred)
{
}
