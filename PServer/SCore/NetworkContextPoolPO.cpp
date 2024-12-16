#include "stdafx.hxx"
#include "NetworkContextPoolPO.hxx"
#include "NetworkContextPO.hxx"

NetworkContextPoolPO::NetworkContextPoolPO(size_t _reserveSize)
{
    Reserve(_reserveSize);
}

NetworkContextPoolPO::~NetworkContextPoolPO()
{
    AutoLock(m_xLock);

    for (auto each : m_oAllocatedList)
    {
        SafeDelete(each);
    }

    m_oAllocatedList.clear();
    m_oFreeList.clear();
}

NetworkContextPO* NetworkContextPoolPO::Allocate()
{
    NetworkContextPO* lContext = nullptr;
    {
        AutoLock(m_xLock);
        if (m_oFreeList.empty() == true)
        {
            //메모리 할당
            lContext = new NetworkContextPO;
            m_oAllocatedList.push_back(lContext);
            m_nAllocatedSize++;
        }
        else
        {
            //m_oFreeList의 맨 첫 데이터를 반환
            lContext = m_oFreeList.front();

            //m_oFreeList의 맨 첫 데이터를 삭제
            m_oFreeList.pop_front();
        }
    }

    lContext->IncreaseReferenceCount();

    return lContext;
}

void NetworkContextPoolPO::Release(NetworkContextPO* _ctxt)
{
    if (nullptr == _ctxt) return;

    if (_ctxt->DecreaseReferenceCount() > 0) return;

    _ctxt->Reset();

    AutoLock(m_xLock);
    m_oFreeList.push_back(_ctxt);
}

void NetworkContextPoolPO::Reserve(size_t _size)
{
    if (_size <= m_nAllocatedSize)
        return;

    size_t lGap = _size - m_nAllocatedSize;

    for (size_t i = 0; i < lGap; ++i)
    {
        auto lpContext = new NetworkContextPO;
        m_oAllocatedList.push_back(lpContext);
        m_oFreeList.push_back(lpContext);
    }

    m_nAllocatedSize = _size;
}

const size_t NetworkContextPoolPO::GetAllocatedCount() const
{
    return m_nAllocatedSize;
}

void NetworkContextPoolPO::GetUsage(size_t& _free, size_t& _allocated)
{
    _allocated = m_nAllocatedSize;
    AutoLock(m_xLock);
    _free = m_oFreeList.size();
}
