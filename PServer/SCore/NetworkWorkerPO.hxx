#pragma once
#include <vector>

class NetworkContext;
class NetworkHost;

class NetworkWorkerPO
{
private:
    volatile bool m_bIsTerminated = false;

    HANDLE m_hIOCP = INVALID_HANDLE_VALUE;

};

