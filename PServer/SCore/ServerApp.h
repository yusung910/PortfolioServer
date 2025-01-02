#pragma once
#include "SCoreAPI.h"
#include <functional>
#include <memory>

struct ListenerInfo;
class ServerAppPO;
class NetworkEventSync;


class SCoreAPI ServerApp
{
private:
    ServerAppPO* m_pServerApp;

public:
    ServerApp();
    virtual ~ServerApp();

    void SetListenerInfo(const ListenerInfo& _info);

    void BindEventSync(std::shared_ptr<NetworkEventSync> _eventSync);

    void RegisterCommand(int _cmd, std::function<void(void)> _func);
    void ProcessQuit();

    void Run();
};

