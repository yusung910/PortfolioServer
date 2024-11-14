#include "stdafx.hxx"
#include "NetworkManager.h"
#include "NetworkManagerPO.hxx"

NetworkManager::NetworkManager()
{
}


NetworkManager::~NetworkManager()
{
    SafeDelete(m_pManager);
}

void NetworkManager::Initialize()
{
    m_pManager = new NetworkManagerPO();
}

void NetworkManager::ReserveContext(size_t _reserveContext)
{
    if (nullptr != m_pManager)
        m_pManager->ReserveContext(_reserveContext);
}

size_t NetworkManager::GetContextAllocateCount() const
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextAllocateCount();
}

void NetworkManager::CreateNetwork()
{
    if (nullptr != m_pManager)
        m_pManager->CreateNetwork();
}

void NetworkManager::DestroyNetwork()
{
    if (nullptr != m_pManager)
        m_pManager->DestroyNetwork();
}

bool NetworkManager::Connect(NetworkEventSync* _eventSync, std::string _ip, int _port, int* _pHostID)
{
    if (nullptr == m_pManager)
        return false;
    return m_pManager->Connect(_eventSync, _ip, _port, _pHostID);
}

bool NetworkManager::Listen(NetworkEventSync* _eventSync, std::string _ip, int _port)
{
    if (nullptr == m_pManager)
        return false;
    return m_pManager->Listen(_eventSync, _ip, _port);
}

bool NetworkManager::Join(NetworkEventSync* _eventSync, int _ipaddr, std::string _ip, int _port, SOCKET _sock)
{
    if(nullptr == m_pManager)
        return false;

    return m_pManager->Join(_eventSync, _ipaddr, _ip, _port, _sock);
}

bool NetworkManager::Send(const int& _hostID, Packet::SharedPtr _packet)
{
    if (nullptr == m_pManager)
        return false;

    return m_pManager->Send(_hostID, _packet);
}

bool NetworkManager::BroadCast(std::vector<int>& _hostIDs, Packet::SharedPtr _packet)
{
    if (nullptr == m_pManager)
        return false;

    return m_pManager->BroadCast(_hostIDs, _packet);
}

bool NetworkManager::Close(const int& _hostID)
{
    if (nullptr == m_pManager)
        return false;

    return m_pManager->Close(_hostID);
}

bool NetworkManager::CloseHost(int _hostID, const std::string& _strReason)
{
    if (nullptr == m_pManager)
        return false;

    return m_pManager->CloseHost(_hostID, _strReason);
}

std::string NetworkManager::GetIP(int _hostID)
{
    if (nullptr == m_pManager)
        return "";

    return m_pManager->GetIP(_hostID);
}

int NetworkManager::GetIPInt32(int _hostID)
{
    if (nullptr == m_pManager)
        return 0;
    return m_pManager->GetIPInt32(_hostID);
}

int NetworkManager::GetConnectorHostID(const std::string& _ip, int _port)
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetConnectorHostID(_ip, _port);
}

int64_t NetworkManager::GetLastPacketTick(int _hostID)
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetLastPacketTick(_hostID);
}

//----------------------------------------------------------
//NetworkContext Pool
//----------------------------------------------------------

NetworkContextPO* NetworkManager::AllocateContext()
{
    if(nullptr == m_pManager)
        return nullptr;
    return m_pManager->AllocateContext();
}

void NetworkManager::ReleaseContext(NetworkContextPO* _ctxt)
{
    if (nullptr != m_pManager)
        m_pManager->ReleaseContext(_ctxt);
}

int NetworkManager::GetContextAllocateCount()
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextAllocateCount();
}

int NetworkManager::GetContextUseCount()
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextUseCount();
}

int NetworkManager::GetContextFreeCount()
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextFreeCount();
}

//----------------------------------------------------------
//NetworkContext Pool end
//----------------------------------------------------------

//----------------------------------------------------------
//NetworkHost Pool begin
//----------------------------------------------------------
NetworkHostPO* NetworkManager::AllocateHost()
{
    if (nullptr == m_pManager)
        return nullptr;

    return m_pManager->AllocateHost();
}

void NetworkManager::ReleaseHost(NetworkHostPO* _host)
{
    if (nullptr != m_pManager)
        m_pManager->ReleaseHost(_host);
}

bool NetworkManager::CheckHost(int _hostID)
{
    if (nullptr == m_pManager)
        return false;
    return m_pManager->CheckHost(_hostID);
}
//----------------------------------------------------------
//NetworkHost Pool end
//----------------------------------------------------------

//----------------------------------------------------------
//NetworkWorker begin
//----------------------------------------------------------

bool NetworkManager::RegisterWorker(NetworkHostPO* _host)
{
    if (nullptr == m_pManager)
        return false;
    return m_pManager->RegisterWorker(_host);
}

bool NetworkManager::DispatchWorker(NetworkHostPO* _host, NetworkContextPO* _ctxt)
{
    if (nullptr == m_pManager)
        return false;
    return m_pManager->DispatchWorker(_host, _ctxt);
}

//----------------------------------------------------------
//NetworkWorker Pool begin
//----------------------------------------------------------
// 
//----------------------------------------------------------
//NetworkStatics begin
//----------------------------------------------------------
void NetworkManager::OnConnect(int _hostID)
{
    if (nullptr != m_pManager)
        m_pManager->OnConnect(_hostID);
}

void NetworkManager::OnDisconnect(int _hostID)
{
    if (nullptr != m_pManager)
        m_pManager->OnDisconnect(_hostID);
}

void NetworkManager::OnSend(const int& _bytes)
{
    if (nullptr != m_pManager)
        m_pManager->OnSend(_bytes);
}

void NetworkManager::OnRecv(const int& _bytes)
{
    if (nullptr != m_pManager)
        m_pManager->OnRecv(_bytes);
}

bool NetworkManager::IsConnected(const int& _hostID)
{
    if(nullptr == m_pManager)
        return false;

    return m_pManager->IsConnected(_hostID);
}
NetworkStatistics* NetworkManager::GetStatistics()
{
    if (nullptr == m_pManager)
        return nullptr;
    return m_pManager->GetStatistics();
}
void NetworkManager::GetConnectedList(std::vector<int>& _list)
{
    if (nullptr != m_pManager)
        m_pManager->GetConnectedList(_list);
}
bool NetworkManager::IsInitialized() const
{
    if (nullptr == m_pManager)
        return false;

    return m_pManager->IsInitialized();
}
size_t NetworkManager::GetContextUsingCount()
{
    if (nullptr == m_pManager)
        return 0;
    return m_pManager->GetContextUsingCount();
}
void NetworkManager::SetClientHostMode(const int& _hostID, const bool& _onoff)
{
    if (nullptr != m_pManager)
        m_pManager->SetClientHostMode(_hostID, _onoff);
}
//----------------------------------------------------------
//NetworkStatics end
//----------------------------------------------------------