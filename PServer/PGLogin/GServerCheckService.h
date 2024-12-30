#pragma once
#include "WaitingList.h"

#include <RefSingleton.h>
#include <Service.h>
#include <vector>
#include <array>
#include <unordered_map>

class GServerCheckService : public Service, public RefSingleton<GServerCheckService>
{
private:
    std::mutex m_xWaitingLock;
    std::unordered_map <int, WaitingList> m_umWaitingList;

};

