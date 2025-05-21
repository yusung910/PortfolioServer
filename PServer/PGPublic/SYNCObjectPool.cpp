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

SYNCObject* SYNCObjectPool::Pop(const EObjectBehavior::Type& _behav)
{
    AutoLock(m_xLock);

    SYNCObject* localSync;
    if (m_oPoolArray.at(_behav).empty())
    {
        localSync = Create(_behav);
    }
    else
    {
        localSync = m_oPoolArray.at(_behav).front();
        m_oPoolArray.at(_behav).pop_front();
    }

    localSync->Reset();
    return localSync;
}

void SYNCObjectPool::Push(SYNCObject* _sync)
{
    AutoLock(m_xLock);
    m_oPoolArray.at(_sync->GetBehaviorType()).push_back(_sync);
}

void SYNCObjectPool::AddCreateSYNCObjectDUserBuff(std::vector<std::shared_ptr<SYNCObjectDUserBuff>>& _syncList, const int& _skillModuleID, const int64_t& remainMS, const int& _opt, const int& _val1, const int& _val2, const int& _val3, const int& _val4, const int& _val5)
{
}
