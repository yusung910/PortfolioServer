#include "stdafx.hxx"
#include "NetworkContextPoolPO.hxx"
#include "NetworkContextPO.hxx"

NetworkContextPoolPO::NetworkContextPoolPO(size_t _reserveSize)
{
    Reserve(_reserveSize);
}

NetworkContextPoolPO::~NetworkContextPoolPO()
{
    AutoLock(m_oLock);

    for (auto each : m_oAllocatedList)
    {
        SafeDelete(each);
    }

    m_oAllocatedList.clear();
    m_oFreeList.clear();
}

NetworkContextPO* NetworkContextPoolPO::Allocate()
{
    NetworkContextPO* localContext = nullptr;
    {
        AutoLock(m_oLock);
        if (m_oFreeList.empty() == true)
        {
            //메모리 할당
            localContext = new NetworkContextPO;
            m_oAllocatedList.push_back(localContext);
            m_nAllocatedSize++;
        }
        else
        {
            //m_oFreeList의 맨 첫 데이터를 반환
            localContext = m_oFreeList.front();

            //m_oFreeList의 맨 첫 데이터를 삭제
            m_oFreeList.pop_front();
        }
    }

    localContext->IncreaseReferenceCount();

    return localContext;
}

void NetworkContextPoolPO::Release(NetworkContextPO* _ctxt)
{
    if (nullptr == _ctxt) return;

    if (_ctxt->DecreaseReferenceCount() > 0) return;

    _ctxt->Reset();

    AutoLock(m_oLock);
    m_oFreeList.push_back(_ctxt);
}

void NetworkContextPoolPO::Reserve(size_t _size)
{
    if (_size <= m_nAllocatedSize)
        return;

    size_t localGap = _size - m_nAllocatedSize;

    for (size_t i = 0; i < localGap; ++i)
    {
        auto localpContext = new NetworkContextPO;
        m_oAllocatedList.push_back(localpContext);
        m_oFreeList.push_back(localpContext);
    }

    m_nAllocatedSize = _size;
}

const size_t NetworkContextPoolPO::GetAllocatedCount() const
{
    return m_nAllocatedSize;
}

void NetworkContextPoolPO::GetUsage(size_t& _free, size_t& _allocated)
{
    _free = m_nAllocatedSize;
    AutoLock(m_oLock);
    _allocated = m_oFreeList.size();
}
