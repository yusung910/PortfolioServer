#pragma once
#include <RefSingleton.h>

#include "SYNCObject.h"
#include "SYNCObjectAdd.h"


class SYNCObjectPool : public RefSingleton<SYNCObjectPool>
{
public:
    std::mutex m_xLock;

    std::array<std::deque<SYNCObject*>, EObjectBehavior::Max> m_oPoolArray;
    std::array<int, EObjectBehavior::Max> m_oPoolCountArray;

public:
    SYNCObjectPool();
    ~SYNCObjectPool();

    void Reserve();
    void Reserve(const EObjectBehavior::Type& _behav, const int& _size);

    SYNCObject* Create(const EObjectBehavior::Type& _behav);
    SYNCObject* Pop(const EObjectBehavior::Type& _behav);

    void Push(SYNCObject* _sync);

    void AddCreateSYNCObjectDUserBuff
    (
        std::vector<std::shared_ptr<SYNCObjectDUserBuff>>& _syncList
        , const int& _skillModuleID
        , const int64_t& remainMS
        , const int& _opt = 0
        , const int& _val1 = 0
        , const int& _val2 = 0
        , const int& _val3 = 0
        , const int& _val4 = 0
        , const int& _val5 = 0
    );
};

