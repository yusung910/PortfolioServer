#include "stdafx.hxx"
#include "Timer.h"
#include "TimerPO.hxx"

Timer::Timer()
{
    m_pTimer = new TimerPO();
}

Timer::~Timer()
{
    SafeDelete(m_pTimer);
}

bool Timer::RegisterTimer(const int& _intervalMS, std::function<void()> _callback)
{
    if (nullptr == m_pTimer)
        return false;
    return m_pTimer->RegisterTimer(_intervalMS, _callback);
}

void Timer::UpdateTimer()
{
    if (nullptr != m_pTimer)
        m_pTimer->UpdateTimer();
}

bool Timer::ChangeTimerInterval(const int& _updateIntervalMS)
{
    if (nullptr == m_pTimer)
        return false;
    return m_pTimer->ChangeTimerInterval(_updateIntervalMS);
}


