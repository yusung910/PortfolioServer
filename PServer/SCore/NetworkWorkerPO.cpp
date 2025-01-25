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

    // IOCP �ڵ� ����
    // CreateIoCompletionPort() : IOCP Ŀ�� ��ü�� �����ϰų� IOCP�� ����̽��� �����ϴ� �۾��� ����. 
    // https://learn.microsoft.com/ko-kr/windows/win32/fileio/createiocompletionport
    auto lIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    if (lIOCP == NULL || lIOCP == INVALID_HANDLE_VALUE)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::CreateThread() CreateIoCompletionPort() Failed:%d", GetLastError());

        return false;
    }

    m_hIOCP = lIOCP;

    //������ ����
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

    //PostQueuedCompletionStatus() ���ڰ����� ���� ���� ���� ��Ŷ���� ����
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

    //���ڰ����� ������ _host, _ctxt�� ����� ���� ��Ŷ���� �����Ѵ�
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

    // IOCP �ڵ� ����
    // CreateIoCompletionPort() : IOCP Ŀ�� ��ü�� �����ϰų� IOCP�� ����̽��� �����ϴ� �۾��� ����. 
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
        // �� �Լ��� ȣ���ϸ� ȣ���� ������� I/O completion queue���� �����͸� �����´� 
        // ���� �����Ͱ� ���� ��� �����Ͱ� ���� �� �� ���� ���.
        //BOOL GetQueuedCompletionStatus(
        //HANDLE       CompletionPort,
        //    LPDWORD      lpNumberOfBytesTransferred,
        //    PULONG_PTR   lpCompletionKey,
        //    LPOVERLAPPED* lpOverlapped,
        //    DWORD        dwMilliseconds
        // )
        //  CompletionPort : ��⸦ ������ ��� IOCP �ڵ��� �Է�
        //  lpNumberOfBytesTransferred : �ۼ��ŵ� ����Ʈ ��(output)
        //  lpCompletionKey	: �񵿱� I / O ��û�� �߻��� ����̽��� completionKey(output)
        //  lpOverlapped : �񵿱� ȣ��� ������ Overlapped ����ü�� �ּ�(output)
        //  dwMilliseconds : ��⸦ ������ �ð�(�и���)�� �Է��մϴ�.
        //    return = TRUE ����, FALSE ����
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
            // ERROR_NETNAME_DELETED: Ŭ���̾�Ʈ�� HardClose�� ��� �߻�
            // ������ ���� ����
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

        // Error Exception ó��
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
                case ERROR_CONNECTION_ABORTED: // ���� ������. Socket Clost �޼��� ����
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
    //Accept �Ϸ� ó��
    SOCKET lSocket = INVALID_SOCKET;
    _ctxt.Read(&lSocket, sizeof(lSocket));

    if (_rslt == true)
    {
        //Accept ���� �� �ּ�����������(sizeof(SOCKADDR_IN) + 16 * 2)�� �ø���
        _ctxt.Write((sizeof(SOCKADDR_IN) + 16 * 2));

        //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-setsockopt
        //������ �ɼ��� SO_UPDATE_ACCEPT_CONTEXT ������ �����Ѵ� 
        SOCKET lListener = _host.GetSocket();
        if (setsockopt(lSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&lListener, sizeof(lListener)) == SOCKET_ERROR)
        {
            VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessAccept() Failed - SO_UPDATE_ACCEPT_CONTEXT: %d", WSAGetLastError());
        }

        //���� �ּ� ����
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

    //NetworkContextPO �ʱ�ȭ
    _ctxt.ResetBuffer();

    //�ٽ� Accept ��û
    if (_host.Accept(_ctxt) == false)
    {
        //Listen Socket�� �����Ű�� �ʱ� ���� Accept�� �����ص� �������� ó��.
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

    //���� �̺�Ʈ ȣ��
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
    //��û ��� üũ
    if (_rslt == false || _transferred <= 0)
    {
        //NetworkHostType�� EHostType::Acceptor�� �ƴϰ�(and)
        //_rslt ���� false �̸�(and)
        //_transferred�� ���� 0���� Ŭ(_transferred > 0) ���(and)
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

    //Receive �� ũ�� �߰�
    _ctxt.Write(_transferred);

    //��ȣȭ ó��
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
    //��û ��� üũ
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
    //��û ��� üũ
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
    //Transferred 0�� ��쿡�� �ϴ� ����
    if (_rslt == false)
    {
        VIEW_WRITE_ERROR(L"NetworkWorkerPO::ProcessSend() Failed - Result is False");
        _host.EndSendTask(false);
        return;
    }
#endif // SEALED_SEND_SUCCESS_TRANSFERRED_0

    // ���� ���� üũ
    if (_ctxt.GetDataSize() != static_cast<size_t>(_transferred))
    {
        VIEW_WRITE_WARNING(L"NetworkWorkerPO::ProcessSend() - Transferred:%d, %d", _ctxt.GetDataSize(), _transferred);
    }

    if (_host.GetHostType() != EHostType::Connector)
        NetworkManager::GetInst().OnSend(_transferred);

    //Send �Ϸ� ó��
    _host.EndSendTask(true);
}
