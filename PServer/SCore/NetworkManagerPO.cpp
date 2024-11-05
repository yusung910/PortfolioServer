#include "stdafx.hxx"
#include "NetworkManagerPO.hxx"

//
#include "NetworkContextPO.hxx"
#include "NetworkContextPoolPO.hxx"
//
#include "NetworkHostPO.hxx"
#include "NetworkHostPoolPO.hxx"
//
#include "NetworkWorkerPO.hxx"
//
#include "NetworkControllerPO.hxx"
//
#include "NetworkStatistics.h"

#define CreateContext(x)    auto x = AllocateContext();     \
                            if(x == nullptr) return false;  \


//----------------------------------------------------------
// NetworkManagerPO Constructor, Destructor inner method
//----------------------------------------------------------
NetworkManagerPO::NetworkManagerPO(size_t _reserveContext)
{
    m_pContextPool = new NetworkContextPoolPO(_reserveContext);
    m_pHostPool = new NetworkHostPoolPO();
    m_pController = new NetworkControllerPO();
    m_pWorker = new NetworkWorkerPO();
    m_pStatistics = new NetworkStatistics();
}

NetworkManagerPO::~NetworkManagerPO()
{
    SafeDelete(m_pContextPool);
    SafeDelete(m_pController);
    SafeDelete(m_pHostPool);
    SafeDelete(m_pWorker);
    SafeDelete(m_pStatistics);
}

void NetworkManagerPO::ReserveContext(size_t _reserveContext)
{
    if (nullptr == m_pContextPool)
        return;
    m_pContextPool->Reserve(_reserveContext);
}

size_t NetworkManagerPO::GetContextAllocateCount() const
{
    if (nullptr == m_pContextPool)
        return;
    return m_pContextPool->GetAllocatedCount();
}

void NetworkManagerPO::CreateNetwork()
{

}

void NetworkManagerPO::DestroyNetwork()
{

}
//----------------------------------------------------------
// NetworkManagerPO Constructor, Destructor inner method end
//----------------------------------------------------------

//----------------------------------------------------------
// NetworkContextPool
//----------------------------------------------------------

NetworkContextPO* NetworkManagerPO::AllocateContext()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::AllocateContext() Failed - NetworkContext Pool is null!");
        return nullptr;
    }
    return m_pContextPool->Allocate();
}

void NetworkManagerPO::ReleaseContext(NetworkContextPO* _context)
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::ReleaseContext() Failed - NetworkContext Pool is null!");
        return;
    }

    m_pContextPool->Release(_context);
}

int NetworkManagerPO::GetContextAllocateCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetContextAllocateCount() Failed - NetworkContext Pool is null!");
        return 0;
    }
    return (int) m_pContextPool->GetAllocatedCount();
}

int NetworkManagerPO::GetContextUseCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetContextUseCount() Failed - NetworkContext Pool is null!");
        return 0;
    }
    size_t localAlloc = 0ll;
    size_t localFree = 0ll;

    m_pContextPool->GetUsage(localFree, localAlloc);

    return (int)(localAlloc - localFree);
}

int NetworkManagerPO::GetContextFreeCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetContextFreeCount() Failed - NetworkContext Pool is null!");
        return 0;
    }

    return 0;
}
//----------------------------------------------------------
//NetworkHost Pool begin
//----------------------------------------------------------

NetworkHostPO* NetworkManagerPO::AllocateHost()
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::AllocateHost() Failed - HostPool is nullptr ");
        return nullptr;
    }
    return m_pHostPool->Allocate();
}

void NetworkManagerPO::ReleaseHost(NetworkHostPO* _host)
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::ReleaseHost() Failed - HostPool is nullptr ");
        return;
    }

    m_pHostPool->Release(_host);
}

bool NetworkManagerPO::CheckHost(int _hostID)
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::CheckHost() Failed - HostPool is nullptr ");
        return false;
    }
    return m_pHostPool->Check(_hostID);
}

//----------------------------------------------------------
//NetworkHost Pool end
//----------------------------------------------------------


//----------------------------------------------------------
//NetworkWorker begin
//----------------------------------------------------------

bool NetworkManagerPO::RegisterWorker(NetworkHostPO* _host)
{
    return m_pWorker->RegisterThread(_host);
}

bool NetworkManagerPO::DispatchWorker(NetworkHostPO* _host, NetworkContextPO* _ctxt)
{
    if (m_pWorker->PushThread(_host, _ctxt) == false)
    {
        VIEW_WRITE_ERROR("Worker PushThread() is Failed");
        return false;
    }
    return true;
}

//----------------------------------------------------------
//NetworkWorker end
//----------------------------------------------------------

//----------------------------------------------------------
//NetworkStatics begin
//----------------------------------------------------------
void NetworkManagerPO::OnConnect(int _hostID)
{
    AutoLock(m_xConnectionListLock);
    auto it = m_oUsingHostIDList.find(_hostID);

    if (it != m_oUsingHostIDList.end())
        return;

    m_oUsingHostIDList.insert(_hostID);

    if (nullptr != m_pStatistics)
        m_pStatistics->Connect();

}

void NetworkManagerPO::OnDisconnect(int _hostID)
{
    AutoLock(m_xConnectionListLock);
    auto it = m_oUsingHostIDList.find(_hostID);

    if (it != m_oUsingHostIDList.end())
        return;

    m_oUsingHostIDList.erase(it);

    if (nullptr != m_pStatistics)
        m_pStatistics->Disconnect();
}

void NetworkManagerPO::OnSend(const int& _bytes)
{
    if (_bytes <= 0)
        return;
    if (nullptr != m_pStatistics)
        m_pStatistics->Send(_bytes);
}

void NetworkManagerPO::OnRecv(const int& _bytes)
{
    if (_bytes <= 0)
        return;
    if (nullptr != m_pStatistics)
        m_pStatistics->Recv(_bytes);
}

bool NetworkManagerPO::IsConnected(const int& _hostID)
{
    AutoLock(m_xConnectionListLock);
    return m_oUsingHostIDList.find(_hostID) != m_oUsingHostIDList.end(_hostID);
}

NetworkStatistics* NetworkManagerPO::GetStatistics()
{
    return m_pStatistics;
}

void NetworkManagerPO::GetConnectedList(std::vector<int>& _list)
{
    AutoLock(m_xConnectionListLock);
    _list.clear();
    _list.assign(m_oUsingHostIDList.begin(), m_oUsingHostIDList.end());
}

//----------------------------------------------------------
//NetworkStatics end
//----------------------------------------------------------