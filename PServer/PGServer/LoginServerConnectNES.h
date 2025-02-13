#pragma once
#include <NetworkEventSync.h>
class LoginServerConnectNES : public NetworkEventSync
{
public:
    LoginServerConnectNES() = default;
    virtual ~LoginServerConnectNES() = default;

    virtual void OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType = 0);
    virtual void OnClose(const int& _hostID);
    virtual void OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize);

    virtual void OnListen() override;
    virtual void OnListenFailed() override;

};

