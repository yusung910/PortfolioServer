#include "stdafx.hxx"
#include "NetworkControllerPO.hxx"

#include "NetworkManager.h"
#include "NetworkContextPO.hxx"
#include "NetworkHostPO.hxx"

#include "Clock.h"

NetworkControllerPO::NetworkControllerPO()
    : m_bIsTerminated(false)
    , m_hNetworkControl(INVALID_HANDLE_VALUE)
{
    RegisterTimer(0, std::bind(&NetworkControllerPO::_UpdateHost, this));
}

NetworkControllerPO::~NetworkControllerPO()
{
    TerminateThread();
    if (m_hNetworkControl != INVALID_HANDLE_VALUE
        && m_hNetworkControl != NULL)
    {
        WaitForSingleObject(m_hNetworkControl, INFINITE);
        CloseHandle(m_hNetworkControl);
    }

    AutoLock(m_xHostIDLock);
    m_oHostList.clear();
}

bool NetworkControllerPO::CreateThread()
{
    //������ �ڵ� ����
    auto localHandle = (HANDLE)_beginthreadex(nullptr, 0, ExcuteThread, this, 0, nullptr);
    
    if (localHandle == nullptr
        || localHandle == INVALID_HANDLE_VALUE)
    {
        TerminateThread();
        return false;
    }

    m_hNetworkControl = localHandle;
    

    //���
    Sleep(100);

    if (true == m_bIsTerminated)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::CreateThread() - Failed");
        return false;
    }

    return true;
}

void NetworkControllerPO::TerminateThread()
{
    m_bIsTerminated = true;
}

unsigned int __stdcall NetworkControllerPO::ExcuteThread(void* _arg)
{
    auto& localParent = *(NetworkControllerPO*)_arg;

    localParent.TerminateThread();
    localParent.ProcessThread();
    return 0;
}

bool NetworkControllerPO::PushThread(NetworkContextPO* _ctxt)
{
    if (_ctxt == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkControllerPO::PushThread() Failed - Parameter is nullptr");
        return false;
    }

    m_oMsgQueue.Push(_ctxt);
    return true;
}

int NetworkControllerPO::GetConnectorHostID(const std::string& _ip, int _port)
{
    for (auto it : m_ConnectorHostList)
    {
        if (true == it.IsSame(_ip, _port))
            return it.m_nHostID;
    }
    return 0;
}

bool NetworkControllerPO::SendPacketToHost(const int& _hostID, Packet::SharedPtr _packet)
{
    if (nullptr == _packet)
        return false;

    auto localHost = _FindHost(_hostID);

    if (nullptr == localHost)
        return false;

    return localHost->Waiting(_packet);
}

bool NetworkControllerPO::_AddHost(NetworkHostPO* _host)
{
    if (_host == nullptr)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, Parameter is nullptr");

        return false;
    }

    {
        AutoLock(m_xHostIDLock);

        auto localIter = m_oHostList.find(_host->GetHostID());
        if (localIter != m_oHostList.end())
        {
            //m_oHostList�� ��ϵǾ� ���� ��� �����ϰ� �Լ� ����
            VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, Duplicated HostID (%d)", _host->GetHostID());
            _host->Close(ESocketCloseType::AddFailHostMap);
            _host->EventClose();
            NetworkManager::GetInst().ReleaseHost(_host);
            return false;
        }

        m_oHostList[_host->GetHostID()] = _host;
    }

    //���� ����
    SOCKET localSocket = _host->GetSocket();
    if (localSocket == INVALID_SOCKET)
    {
        // WSASocket ���� ����, ���� �ɼ� ����
        // https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsasocketw
        localSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

        if (localSocket == INVALID_SOCKET)
        {
            VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, WSASocket:%d", WSAGetLastError());
            _host->Close(ESocketCloseType::SocketCreateFail);
            return false;
        }

        _host->SetSocket(localSocket);

    }
    if (NetworkManager::GetInst().RegisterWorker(_host) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed Bind error");
        _host->Close(ESocketCloseType::RegisterWorkerFail);

        return false;
    }

    //���ڰ����� ���޹��� NetworkHostPO�� ������� m_ConnectorHostList�� ����Ѵ�
    _AddConnectorHost(_host);

    return true;
}

NetworkHostPO* NetworkControllerPO::_FindHost(int _hostID)
{
    AutoLock(m_xHostIDLock);
    auto localIter = m_oHostList.find(_hostID);
    if (localIter == m_oHostList.end())
        return nullptr;
    return localIter->second;
}

void NetworkControllerPO::_UpdateHost()
{
    int64_t localAppTimeMS = Clock::GetTick64();
    AutoLock(m_xHostIDLock);

    auto localIter = m_oHostList.begin();
    while (localIter != m_oHostList.end())
    {
        auto localHost = localIter->second;
        if (localHost)
        {
            if (localHost->IsAlive())
            {
                localHost->Update(localAppTimeMS);
                localIter++;
                continue;
            }

            localHost->EventClose();
            NetworkManager::GetInst().ReleaseHost(localHost);
        }
    }
}

void NetworkControllerPO::_AddConnectorHost(NetworkHostPO* _host)
{
    if (nullptr == _host)
        return;

    for (auto& it : m_ConnectorHostList)
    {
        if (it.m_nHostID == _host->GetHostID())
            return;
    }

    ConnectorTargetInfo localInfo;
    localInfo.m_nHostID = _host->GetHostID();
    localInfo.SetIP(_host->GetIP());
    localInfo.m_nPort = _host->GetPeerPort();
    m_ConnectorHostList.push_back(localInfo);
}

void NetworkControllerPO::_RemoveConnectorHost(int _hostID)
{
    for (auto it = m_ConnectorHostList.begin(); it != m_ConnectorHostList.end(); ++it)
    {
        if (it->m_nHostID == _hostID)
        {
            m_ConnectorHostList.erase(it);
            return;
        }
    }
}

void NetworkControllerPO::ProcessThread()
{
    while (m_bIsTerminated)
    {
        //Timer Ŭ������ RegisterTimer() �Լ��� ���� ��ϵ� �Լ��� �����Ѵ�.
        //���� Ŭ���� �����ڿ� RegisterTimer()�� �����Ѵ�.
        UpdateTimer();

        auto localCtxt = m_oMsgQueue.Pop();
        if (localCtxt == nullptr)
        {
            Sleep(1);
            continue;
        }

        switch (localCtxt->GetContextType())
        {
        case EContextType::Connect: ProcessConnect(*localCtxt);  break;
        case EContextType::Listen:  ProcessListen(*localCtxt);   break;
        case EContextType::Join:    ProcessJoin(*localCtxt);     break;
        case EContextType::Close:   ProcessClose(*localCtxt);     break;
        default:
        {
            VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessThread() Failed - GetContextType:%d", localCtxt->GetContextType());
        }
        break;
        }

        NetworkManager::GetInst().ReleaseContext(localCtxt);
    }
}

void NetworkControllerPO::ProcessConnect(NetworkContextPO& _ctxt)
{
    //���ڰ����� ���޹��� NetworkContextPO ��ü�� �����͸� ���������� �����Ѵ�
    NetworkHostPO* localHost = nullptr;
    _ctxt.Read(&localHost, sizeof(localHost));

    //NetworkHost ����
    if (_AddHost(localHost) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessConnect() - Failed : _AddHost()");
        return;
    }

    //NetworkContextPO �ʱ�ȭ
    _ctxt.ResetBuffer();

    //Connect ��û
    localHost->BeginBaseTask();

    if (localHost->Connect(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessConnect() - Failed : NetworkHostPO->Connect()");

        localHost->EndBaseTask(false);
    }
}

void NetworkControllerPO::ProcessListen(NetworkContextPO& _ctxt)
{
    //���ڰ����� ���޹��� NetworkContextPO ��ü�� �����͸� ���������� �����Ѵ�
    NetworkHostPO* localHost = nullptr;
    _ctxt.Read(&localHost, sizeof(localHost));

    if (_AddHost(localHost) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessListen() - Failed : _AddHost()");
        return;
    }


    //Host Listen ��û
    if (localHost->Listen() == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessListen() - Failed : Host->Listen()");

        localHost->Close(ESocketCloseType::ListenFail);
        return;
    }
}

void NetworkControllerPO::ProcessJoin(NetworkContextPO& _ctxt)
{
    //���ڰ����� ���޹��� NetworkContextPO ��ü�� �����͸� ���������� �����Ѵ�
    NetworkHostPO* localHost = nullptr;
    _ctxt.Read(&localHost, sizeof(localHost));

    if (_AddHost(localHost) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessJoin() - Failed : _AddHost()");
        return;
    }

    //���� �̺�Ʈ ȣ��
    localHost->EventConnect(EHostType::Acceptor);

    //Context Reset
    _ctxt.ResetBuffer();

    //Receive
    localHost->BeginBaseTask();

    if (localHost->Receive(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessJoin() - Failed : Receive()");
        localHost->EndBaseTask(false);
        return;
    }
}

void NetworkControllerPO::ProcessClose(NetworkContextPO& _ctxt)
{
    for (auto& hostID : _ctxt.GetHostIDList())
    {
        _RemoveConnectorHost(hostID);
        auto localHost = _FindHost(hostID);

        if (localHost == nullptr) continue;

        localHost->Close(ESocketCloseType::ProcessClose);
    }
}
