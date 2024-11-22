#pragma once
#include <cstdint>
#include <vector>
#include <functional>

struct TimerData;

class TimerPO
{
private:
    int64_t m_nTimerUpdateTimeMS = 0;
    size_t m_nCurrentIdx = 0;
    std::vector<TimerData*> m_oTimerDatas;

    int m_nUpdateInterval = 0;

public:
    TimerPO() = default;
    ~TimerPO();

    bool RegisterTimer(const int& _intervalMS, std::function<void()> _callback);
    void UpdateTimer();
    bool ChangeTimerInterval(const int& _updateIntervalMS);
};

