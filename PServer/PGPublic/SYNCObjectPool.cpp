#include "PGPPrivate.h"

#include "SYNCObjectAdd.h"

#include "SYNCObjectPool.h"

SYNCObjectPool::SYNCObjectPool()
{
    m_oPoolArray.fill(std::deque<SYNCObject*>());
    m_oPoolCountArray.fill(0);

}

SYNCObjectPool::~SYNCObjectPool()
{

}

void SYNCObjectPool::Reserve()
{
    for (auto i = (int)EObjectBehavior::Add; i < (int)EObjectBehavior::Max; i++)
    {
        Reserve((EObjectBehavior::Type)i, 100);
    }
}

void SYNCObjectPool::Reserve(const EObjectBehavior::Type& _behav, const int& _size)
{
    for (int i = 0; i < _size; i++)
    {
        m_oPoolArray.at(_behav).push_back(Create(_behav));
    }
}



SYNCObject* SYNCObjectPool::Create(const EObjectBehavior::Type& _behav)
{
    assert(_behav < EObjectBehavior::Max);
    SYNCObject* localSYNC = nullptr;

    switch (_behav)
    {
    case EObjectBehavior::Add:
        localSYNC = new SYNCObjectAdd();
        break;

    }

    return localSYNC;
}
