#include "stdafx.hxx"
#include "TimerPO.hxx"
#include "TimerData.hxx"
#include "Clock.h"

TimerPO::~TimerPO()
{
    for (auto it = m_oTimerDatas.begin(); it != m_oTimerDatas.end(); ++it)
    {
        SafeDelete(*it);
    }
    m_oTimerDatas.clear();
}

bool TimerPO::RegisterTimer(const int& _intervalMS, std::function<void()> _callback)
{
    if (_intervalMS < 0) return false;
    if (nullptr == _callback) return false;

    TimerData* localTimerData = new TimerData();

    if (nullptr == localTimerData)
        return false;

    localTimerData->Idx = static_cast<int>(++m_nCurrentIdx);
    localTimerData->NextTimeMS = Clock::GetTick64() + _intervalMS;
    localTimerData->CallbackFunc = _callback;
    localTimerData->IntervalTimeMS = _intervalMS;

    m_oTimerDatas.emplace_back(localTimerData);

    return true;
}

void TimerPO::UpdateTimer()
{
    int64_t localCurrentTime = Clock::GetTick64();
    if (m_nTimerUpdateTimeMS > localCurrentTime)
        return;

    for (auto& pData : m_oTimerDatas)
    {
        if (nullptr == pData) continue;

        if (pData->NextTimeMS > localCurrentTime) continue;

        if (nullptr != pData->CallbackFunc)
            pData->CallbackFunc();

        pData->NextTimeMS = localCurrentTime + pData->IntervalTimeMS;
    }

    m_nTimerUpdateTimeMS = localCurrentTime + m_nUpdateInterval;
}

bool TimerPO::ChangeTimerInterval(const int& _updateIntervalMS)
{
    if (_updateIntervalMS <= 0)
        return false;

    m_nUpdateInterval = _updateIntervalMS;

    return true;
}
