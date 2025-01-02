#pragma once
#include <vector>

struct WaitingHostInfo
{
    int m_nWaitingSeq;
    int m_nHostID;
    int m_nTicks;
};

class WaitingList
{
public:
    int m_nAllowedSeq = 0;
    std::vector<int> m_vQueueList = {};

    std::vector<WaitingHostInfo> m_vWaitingDataList;

public:
    WaitingList() = default;
};