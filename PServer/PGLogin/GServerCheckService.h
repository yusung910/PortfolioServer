#pragma once
#include "WaitingList.h"
#include "GameServerInfo.h"

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

    std::unordered_map <int, GameServerInfo*> m_umGameServerList;
    std::unordered_map<int, int> m_umGameServerIDList;
    int m_nLatestGameServerID = 0;
    //
    bool m_bInitialized = false;

    std::mutex m_xServerListLock;
    std::atomic_int m_nTotalServerCount = 0;
    std::atomic_int m_nConnectedSererCount = 0;

public:
    GServerCheckService();
    virtual ~GServerCheckService() = default;

    bool Start();

    bool LoadGameServers();

protected:
    //
    bool OnHostConnect(int _hostID, const HostConnect& _msg);
    bool OnHostClose(int _hostID, const HostClose& _msg);

    //Client -> LoginServer
    bool OnCLAuthReq(int _hostID, const CLAuthReq& _msg);


private:
    //Login -> Game 서버 연결 체크
    void _CheckConnect();

};