#include "stdafx.hxx"
#include "Clock.h"
#include <chrono>

int64_t Clock::GetTick64()
{
    return GetFileTime() / 10000;
}

int64_t Clock::GetFileTime()
{
    FILETIME lFT = {};
    GetSystemTimeAsFileTime(&lFT);

    int64_t lFileTime100ns = ((int64_t)lFT.dwLowDateTime << 32) + (((int64_t)lFT.dwLowDateTime));

    return lFileTime100ns;
}
