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
	m_nMaxConnection = ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_nMaxConnection;

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

bool GameService::OnHostConnect([[maybe_unused]] int _hostID, [[maybe_unused]] const HostConnect& _msg)
{
	if (false == m_bAllowConnect)
	{
		return false;
	}
	VIEW_INFO("GameService::OnHostConnect() %d, %d", _hostID, _msg.messageid());

	NetworkManager::GetInst().SetClientHostMode(_hostID, true);
	return true;
}

bool GameService::OnHostClose(int _hostID, const HostClose& _msg)
{
    VIEW_INFO("HostID(%d) Has Disconnected!(%d)", _hostID, _msg.messageid());
    NetworkManager::GetInst().OnDisconnect(_hostID);
    return false;
}

void GameService::_RegisterPacketHandlers()
{
	RegisterHandler(&GameService::OnHostConnect);
	RegisterHandler(&GameService::OnHostClose);
}

void GameService::_RegisterTimers()
{
}

void GameService::_RegisterSchedules()
{
}
