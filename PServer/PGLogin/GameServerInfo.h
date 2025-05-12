#pragma once
#include <string>
#include <memory>
#include <GlobalEnum.h>
#include "GameServerCheckNES.h"

class GameServerInfo
{
public:
    //fix
    int m_nServerGroup = 0;
    int m_nServerID = 0;

    //내부 IP, Host (bind)
    std::string m_sInboundHost = "127.0.0.1";
    int m_nInboundPort = 0;

    //외부 IP, Host (public)
    std::string m_sOutboundHost = "127.0.0.1";
    int m_nOutboundPort = 35201;

    EServer::Type m_eServerType = EServer::Type::Game;

    //Variable
    EServerStatus::Type m_eServerStatus = EServerStatus::Type::None;
    EServerState::Type m_nServerState = EServerState::Type::None;

    int m_nHostID = 0;

    std::shared_ptr<GameServerCheckNES> m_oHostEvent = std::make_shared<GameServerCheckNES>();

    bool m_bIsConnecting = false;
    bool m_bIsConnected = false;

};