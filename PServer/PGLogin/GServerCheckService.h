#pragma once
#include "WaitingList.h"
#include "GameServerInfo.h"

#include <RefSingleton.h>
#include <Service.h>
#include <vector>
#include <array>

#include <unordered_set>

class GServerCheckService : public Service, public RefSingleton<GServerCheckService>
{
private:
    std::mutex m_xWaitingLock;
    std::unordered_map <int, WaitingList> m_umWaitingList;

    std::unordered_map <int, GameServerInfo*> m_umGameServerList;
    //<hostID, GameServerID
    std::unordered_map<int, int> m_umHIDGSIDList;
    int m_nLatestGameServerID = 0;
    //
    bool m_bInitialized = false;

    std::mutex m_xServerListLock;
    std::atomic_int m_nTotalServerCount = 0;
    std::atomic_int m_nConnectedServerCount = 0;

public:
    GServerCheckService();
    virtual ~GServerCheckService() = default;

    bool Start();

    //서버 접속 대기 인원 수
    int GetServerConnectWaitingPlayerCount(const int& _serverID);
    const int& GetLatestGameServerID() const noexcept;

    GameServerInfo* FindServer(const int& _serverID);
    int GetGServerIDByHostID(const int& _hostID);

    bool LoadGameServers();

    void FillPacketServerList(flatbuffers::FlatBufferBuilder& _fbb, std::vector<flatbuffers::Offset<DServerInfo>>& _vec, const std::unordered_set<int>& _existPilgrimServerList);

    //
    bool SetWaitingEnqueue(const int& _serverID, const int& _hostID);


    void SendPacket(const int& _serverID, const EPacketProtocol& _msgID, void* _msg, const int& _msgSize);
    void SendPacket(const int& _serverID, const EPacketProtocol& _msgID, flatbuffers::FlatBufferBuilder& _fbb);

    int GetTotalServerCount() noexcept;
    int GetConnectedServerCount() noexcept;

protected:
    //
    bool OnHostConnect(int _hostID, const HostConnect& _msg);
    bool OnHostClose(int _hostID, const HostClose& _msg);

private:
    //Login -> Game 서버 연결 체크
    void _CheckConnect();

};