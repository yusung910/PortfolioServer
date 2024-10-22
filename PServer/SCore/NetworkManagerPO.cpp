#include "stdafx.hxx"
#include "NetworkManagerPO.hxx"

//
#include "NetworkContextPO.hxx"
#include "NetworkContextPoolPO.hxx"
//

#define CreateContext(x)    auto x = AllocateContext();     \
                            if(x == nullptr) return false;  \


//----------------------------------------------------------
// NetworkManagerPO Constructor, Destructor inner method
//----------------------------------------------------------
NetworkManagerPO::NetworkManagerPO(size_t _reserveContext)
{
    m_pContextPool = new NetworkContextPoolPO(_reserveContext);
}

NetworkManagerPO::~NetworkManagerPO()
{
    SafeDelete(m_pContextPool);
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
        VIEW_WRITE_ERROR("NetworkContext Pool is null!");
        return nullptr;
    }
    return m_pContextPool->Allocate();
}

void NetworkManagerPO::ReleaseContext(NetworkContextPO* _context)
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkContext Pool is null!");
        return;
    }

    m_pContextPool->Release(_context);
}

int NetworkManagerPO::GetContextAllocateCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkContext Pool is null!");
        return 0;
    }
    return (int) m_pContextPool->GetAllocatedCount();
}

int NetworkManagerPO::GetContextUseCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkContext Pool is null!");
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
        VIEW_WRITE_ERROR("NetworkContext Pool is null!");
        return 0;
    }

    return 0;
}

