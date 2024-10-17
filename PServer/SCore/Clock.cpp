#include "stdafx.hxx"
#include "Clock.h"
#include <chrono>

int64_t Clock::GetTick64()
{
    return GetFileTime() / 10000;
}

int64_t Clock::GetFileTime()
{
    FILETIME localFT = {};
    GetSystemTimeAsFileTime(&localFT);

    int64_t localFileTime100ns = ((int64_t)localFT.dwLowDateTime << 32) + (((int64_t)localFT.dwLowDateTime));

    return localFileTime100ns;
}
