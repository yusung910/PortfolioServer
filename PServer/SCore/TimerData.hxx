#pragma once
#include <cstdint>
#include <functional>

struct TimerData
{
    int                     Idx = 0;
    int64_t                 NextTimeMS = 0;
    int64_t                 IntervalTimeMS = 0;
    std::function<void()>   CallbackFunc = nullptr;
};