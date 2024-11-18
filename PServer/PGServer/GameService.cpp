#include "pch.h"
#include "GameService.h"
#include <ServerConfig.h>
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
}

void GameService::_RegisterPacketHandlers()
{
}

void GameService::_RegisterTimers()
{
}

void GameService::_RegisterSchedules()
{
}
