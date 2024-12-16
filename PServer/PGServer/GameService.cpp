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

bool GameService::OnCSAuthReq(int _hostID, const CSAuthReq& _msg)
{
    VIEW_INFO("OnCSAuthReq _hostID: %d, accountid: %s", _hostID, _msg.accountid()->c_str());

    flatbuffers::FlatBufferBuilder lFB;

    auto lPacket = CreateSCAuthRes(lFB, lFB.CreateString(_msg.accountid()), lFB.CreateString(_msg.accountpw()));
    lFB.Finish(lPacket);

    Packet::SharedPtr lpPacket = Packet::New();
    if (true == lpPacket->SetPacketData(CS_AuthReq, lFB.GetBufferPointer(), lFB.GetSize()))
    {
        NetworkManager::GetInst().Send(_hostID, lpPacket);
        VIEW_INFO("Send Packet to User(hostID: %d) MessageID(%d, %s)", _hostID, CS_AuthReq, EnumNameEPacketProtocol(CS_AuthReq));
    }
    else
    {
        VIEW_WRITE_ERROR("PC(hostID: %d) Send Packet Fail!! MessageID(%d, %s)", _hostID, CS_AuthReq, EnumNameEPacketProtocol(CS_AuthReq));
    }


    return true;
}

bool GameService::OnSCAuthReq(int _hostID, const CSAuthReq& _msg)
{
	VIEW_INFO("OnSCAuthReq _hostID: %d, accountid: %s, pw: %s", _hostID, _msg.accountid()->c_str(), _msg.accountpw());
	return true;
}

void GameService::_RegisterPacketHandlers()
{
	RegisterHandler(&GameService::OnHostConnect);
	RegisterHandler(&GameService::OnCSAuthReq);
	RegisterHandler(&GameService::OnSCAuthReq);
}

void GameService::_RegisterTimers()
{
}

void GameService::_RegisterSchedules()
{
}
