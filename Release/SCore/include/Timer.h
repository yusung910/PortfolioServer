#pragma once
#include "SCoreAPI.h"
#include <functional>
constexpr int UPDATE_INTERVAL_TIME_MS = 10;

class TimerPO;
class Timer
{
private:
    TimerPO* m_pTimer = nullptr;

public:
    Timer();
    virtual ~Timer();

    bool RegisterTimer(const int& _intervalMS, std::function<void()> _callback);
    void UpdateTimer();
    bool ChangeTimerInterval(const int& _updateIntervalMS = UPDATE_INTERVAL_TIME_MS);

};

