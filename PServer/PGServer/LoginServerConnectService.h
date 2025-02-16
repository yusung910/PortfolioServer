#pragma once

#include <RefSingleton.h>
#include <Service.h>
#include <unordered_set>

#include "LoginServerConnectNES.h"

constexpr int TRY_LISTEN_DELAY_MS = 1000;

class LoginServerConnectService : public Service, public RefSingleton<LoginServerConnectService>
{
private:
    LoginServerConnectNES m_oEventSync;
    bool m_bIsStarted = false;
    bool m_bIsTryListening = false;

    int m_nServerID = 0;
    int m_nBindPort = 0;

    std::mutex m_xHostLock;
    std::unordered_set<int> m_umConnectedLoginServeHostIDList;


    int m_nWaitingSeq = 0;
    int m_nAllowedSeq = 0;

    int m_nMaxConnection = 3000;


public:
    LoginServerConnectService();
    virtual ~LoginServerConnectService();

    bool Start();

    void OnListen();
    void OnListenFailed();

    void SendToLoginServers(const int& _msgID, void* _msg, const size_t& _msgSize);

protected:
    bool OnConnect(int _hostID, const HostConnect& _msg);
    bool OnDisconnect(int _hostID, const HostClose& _msg);

    //

    void UpdateServerStatus();

private:
    void _TryListen();
    void _SendToLoginServer(const EPacketProtocol& _msgID, flatbuffers::FlatBufferBuilder& _fbb);
    void _SendToLogin(const int& _hostID, const EPacketProtocol& _msgID, flatbuffers::FlatBufferBuilder& _fbb);

    bool _SendErrorMessage(const int& _hostID, const EErrorMsg& _err, const EPacketProtocol& _msgID);
};

