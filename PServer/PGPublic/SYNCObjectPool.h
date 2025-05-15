#pragma once
#include <RefSingleton.h>

#include "SYNCObject.h"

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
};

