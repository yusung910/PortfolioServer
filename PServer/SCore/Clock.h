#pragma once

#include "SCoreAPI.h"
#include <cstdint>

class SCoreAPI Clock
{
public:
    static int64_t GetTick64();     // 1ms ����
    static int64_t GetFileTime();   // 100ns ����

private:
    Clock() = delete;
    ~Clock() = delete;
};

