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

    TimerData* lTimerData = new TimerData();

    if (nullptr == lTimerData)
        return false;

    lTimerData->Idx = static_cast<int>(++m_nCurrentIdx);
    lTimerData->NextTimeMS = Clock::GetTick64() + _intervalMS;
    lTimerData->CallbackFunc = _callback;
    lTimerData->IntervalTimeMS = _intervalMS;

    m_oTimerDatas.emplace_back(lTimerData);

    return true;
}

void TimerPO::UpdateTimer()
{
    int64_t lCurrentTime = Clock::GetTick64();
    if (m_nTimerUpdateTimeMS > lCurrentTime)
        return;

    for (auto& pData : m_oTimerDatas)
    {
        if (nullptr == pData) continue;

        if (pData->NextTimeMS > lCurrentTime) continue;

        if (nullptr != pData->CallbackFunc)
            pData->CallbackFunc();

        pData->NextTimeMS = lCurrentTime + pData->IntervalTimeMS;
    }

    m_nTimerUpdateTimeMS = lCurrentTime + m_nUpdateInterval;
}

bool TimerPO::ChangeTimerInterval(const int& _updateIntervalMS)
{
    if (_updateIntervalMS <= 0)
        return false;

    m_nUpdateInterval = _updateIntervalMS;

    return true;
}
