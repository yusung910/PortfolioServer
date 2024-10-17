#include "stdafx.hxx"
#include "NetworkManagerPO.hxx"

//
#include "NetworkContextPO.hxx"
#include "NetworkContextPoolPO.hxx"
//

NetworkManagerPO::NetworkManagerPO(size_t _nReserveContext)
{
}

NetworkManagerPO::~NetworkManagerPO()
{
}


//----------------------------------------------------------
//NetworkContextPool
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
