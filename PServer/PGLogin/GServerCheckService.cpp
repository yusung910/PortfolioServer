#include "pch.h"
#include "GServerCheckService.h"
#include "PGConstVars.h"
#include <NetworkManager.h>
#include <ServerConfig.h>

GServerCheckService::GServerCheckService()
{

}

bool GServerCheckService::Start()
{
    RegisterTimer(PG_CHECK_RECONNECT_TIME_MS, std::bind(&GServerCheckService::_CheckConnect, this));
    return false;
}

bool GServerCheckService::LoadGameServers()
{
    if (true == m_bInitialized)
        return false;

    AutoLock(m_xServerListLock);

    auto& lGameServerList = ServerConfig::GetInst().GetConfig().GetGameServerList();

    for (auto it = lGameServerList.begin(); it != lGameServerList.end(); ++it)
    {
        GameServerInfo* lGameServerInfo = new GameServerInfo;
        lGameServerInfo->m_nServerGroup = it->second.m_nServerGroupID;
        lGameServerInfo->m_nServerID = it->second.m_nServerID;

        lGameServerInfo->m_sInboundHost = it->second.m_sBindAddress;
        lGameServerInfo->m_nInboundPort = it->second.m_nBindPort;

        lGameServerInfo->m_eServerType = EServer::Type::Game;
        lGameServerInfo->m_sOutboundHost = it->second.m_sPublicHost;
        lGameServerInfo->m_nOutboundPort = it->second.m_nPublicPort;

        m_umGameServerList.insert_or_assign(lGameServerInfo->m_nServerID, lGameServerInfo);

        if (lGameServerInfo->m_nServerID > m_nLatestGameServerID)
        {
            m_nLatestGameServerID = lGameServerInfo->m_nServerID;
        }
    }

    if (true == m_umGameServerList.empty())
    {
        GameServerInfo* lGameServerInfo = new GameServerInfo;
        lGameServerInfo->m_nServerGroup = 0;
        lGameServerInfo->m_nServerID = 0;

        lGameServerInfo->m_sInboundHost = "127.0.0.1";
        lGameServerInfo->m_nInboundPort = 35202;

        lGameServerInfo->m_eServerType = EServer::Type::Game;
        lGameServerInfo->m_sOutboundHost = "127.0.0.1";
        lGameServerInfo->m_nOutboundPort = 35201;

        m_umGameServerList.insert_or_assign(lGameServerInfo->m_nServerID, lGameServerInfo);
    }

    m_bInitialized = true;

    return true;
}

bool GServerCheckService::OnHostConnect(int _hostID, const HostConnect& _msg)
{
    AutoLock(m_xServerListLock);

    for (auto it = m_umGameServerList.begin(); it != m_umGameServerList.end(); ++it)
    {
        if (nullptr == it->second)
            continue;

        //패킷으로 전달 받은 Port와 접속 확인할 Port 비교
        if (it->second->m_nOutboundPort != _msg.peerport())
            continue;

        //IP 비교
        if (it->second->m_sOutboundHost.compare(_msg.peerip()->c_str()) != 0)
            continue;


        it->second->m_nHostID = _hostID;
        //
        m_umGameServerIDList.insert_or_assign(_hostID, it->second->m_nServerID);
        it->second->m_bIsConnected = true;
        it->second->m_bIsConnecting = false;

        VIEW_WRITE_INFO("Connected To Game(%d) Server [%s:%d, HostID:%d] Success!"
            ,it->second->m_nServerID
            ,it->second->m_sOutboundHost.c_str()
            ,it->second->m_nOutboundPort
            ,_hostID);

        return true;
    }
    return false;
}

bool GServerCheckService::OnHostClose(int _hostID, const HostClose& _msg)
{
    AutoLock(m_xServerListLock);

    auto lServerID = m_umGameServerIDList.find(_hostID);
    if (lServerID != m_umGameServerIDList.end())
        m_umGameServerIDList.erase(lServerID);





    return false;
}

void GServerCheckService::_CheckConnect()
{
}
