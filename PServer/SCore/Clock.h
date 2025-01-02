#pragma once

#include "SCoreAPI.h"
#include <cstdint>

class SCoreAPI Clock
{
public:
    static int64_t GetTick64();     // 1ms 단위
    static int64_t GetFileTime();   // 100ns 단위

private:
    Clock() = delete;
    ~Clock() = delete;
};

