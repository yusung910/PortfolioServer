#pragma once
#include <functional>
#include <memory>
#include <unordered_map>

#include "ServerConfigInfo.h"

class NetworkEventSync;

class ServerAppPO
{
private:
    std::shared_ptr<NetworkEventSync> m_pEventSync = nullptr;
    volatile bool m_bIsRunning = false;
    std::unordered_map<int, std::function<void(void)>> m_oCommandMap;

    ListenerInfo m_oListenerInfo;

public:
    ServerAppPO();
    virtual ~ServerAppPO();

    void SetListenerInfo(const ListenerInfo& _info);

    void BindEventSync(std::shared_ptr<NetworkEventSync> _eventSync);
    void RegisterCommand(int _cmd, std::function<void(void)> _func);
    void ProcessQuit();

    void Run();

private:
    bool _Begin();
    void _Stop();
};

