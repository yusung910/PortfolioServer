#include "pch.h"
#include "GameService.h"
#include <ServerConfig.h>
#include <NetworkManager.h>

#include <LoggingMacro.h>
GameService::GameService()
{
    _RegisterPacketHandlers();	
    _RegisterTimers();			
    _RegisterSchedules();		
}

bool GameService::Start()
{
    m_nCurrentServerID = ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_nServerID;
    m_nMaxConnection= ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_nMaxConnection;

    return CreateThread();
}

void GameService::SetAllowConnect(bool _onoff)
{
    m_bAllowConnect = _onoff;
}

void GameService::OnDeserializeFail(const int& _hostID, const std::string& _hostIP, const int& _failProtocol)
{
    //
    VIEW_ERROR("GameService::OnDeserializeFail() %d, %s, %d", _hostID, _hostIP.c_str(), _failProtocol);
}

bool GameService::OnHostConnect(int _hostID, const HostConnect& _msg)
{
    if (false == m_bAllowConnect)
    {
        return false;
    }
    VIEW_INFO("GameService::OnHostConnect() %d, %s, %d", _hostID, _msg.messageid());

    NetworkManager::GetInst().SetClientHostMode(_hostID, true);
    return true;
}

void GameService::_RegisterPacketHandlers()
{
    RegisterHandler(&GameService::OnHostConnect);
}

void GameService::_RegisterTimers()
{
}

void GameService::_RegisterSchedules()
{
}
