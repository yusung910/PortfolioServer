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
    //쓰레드 핸들 생성
    auto localHandle = (HANDLE)_beginthreadex(nullptr, 0, ExcuteThread, this, 0, nullptr);
    
    if (localHandle == nullptr
        || localHandle == INVALID_HANDLE_VALUE)
    {
        TerminateThread();
        return false;
    }

    m_hNetworkControl = localHandle;
    

    //대기
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
            //m_oHostList에 등록되어 있을 경우 제거하고 함수 종료
            VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, Duplicated HostID (%d)", _host->GetHostID());
            _host->Close(ESocketCloseType::AddFailHostMap);
            _host->EventClose();
            NetworkManager::GetInst().ReleaseHost(_host);
            return false;
        }

        m_oHostList[_host->GetHostID()] = _host;
    }

    //소켓 생성
    SOCKET localSocket = _host->GetSocket();
    if (localSocket == INVALID_SOCKET)
    {
        // WSASocket 소켓 생성, 관련 옵션 참조
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

    //인자값으로 전달받은 NetworkHostPO를 멤버변수 m_ConnectorHostList에 등록한다
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
}
