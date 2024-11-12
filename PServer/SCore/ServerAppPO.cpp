#include "stdafx.hxx"
#include "ServerAppPO.hxx"
#include "NetworkManager.h"
#include <conio.h>
#include "NetworkEventSync.h"

ServerAppPO::ServerAppPO()
{
#ifdef DEV_TEST
    RegisterCommand('q', std::bind(&ServerAppPO::ProcessQuit, this));
#endif // DEV_TEST

}

ServerAppPO::~ServerAppPO()
{
}

void ServerAppPO::SetListenerInfo(const ListenerInfo& _info)
{
    m_oListenerInfo = _info;
}

void ServerAppPO::BindEventSync(std::shared_ptr<NetworkEventSync> _eventSync)
{
    m_pEventSync = _eventSync;
}

void ServerAppPO::RegisterCommand(int _cmd, std::function<void(void)> _func)
{
    auto iter = m_oCommandMap.find(_cmd);
    if (iter != m_oCommandMap.end())
    {
        VIEW_WARNING(L"ServerAppPO::RegisterCommand() Failed - _cmd: %d is Duplicated _cmd", _cmd);
        return;
    }

    m_oCommandMap[_cmd] = _func;
}

void ServerAppPO::ProcessQuit()
{
    m_bIsRunning = false;
}

void ServerAppPO::_Stop()
{
    NetworkManager::GetInst().DestroyNetwork();
}
