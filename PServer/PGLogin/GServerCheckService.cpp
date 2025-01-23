#include "pch.h"
#include "GServerCheckService.h"
#include "PConstVars.h"
#include <NetworkManager.h>
#include <ServerConfig.h>

GServerCheckService::GServerCheckService()
{
    RegisterHandler(&GServerCheckService::OnHostConnect);
    RegisterHandler(&GServerCheckService::OnHostClose);
}

bool GServerCheckService::Start()
{
    RegisterTimer(PG_CHECK_RECONNECT_TIME_MS, std::bind(&GServerCheckService::_CheckConnect, this));

    return CreateThread();
}

int GServerCheckService::GetServerConnectWaitingPlayerCount(const int& _serverID)
{
    AutoLock(m_xWaitingLock);
    auto lIt = m_umWaitingList.find(_serverID);
    if (lIt == m_umWaitingList.end())
        return 0;

    return (int)(lIt->second.m_vQueueList.size() + lIt->second.m_vWaitingDataList.size());
}

const int& GServerCheckService::GetLatestGameServerID() const noexcept
{
    return m_nLatestGameServerID;
}

GameServerInfo* GServerCheckService::FindServer(const int& _serverID)
{
    AutoLock(m_xServerListLock);

    auto lIt = m_umGameServerList.find(_serverID);
    if (lIt == m_umGameServerList.end())
        return nullptr;

    return lIt->second;
}

int GServerCheckService::GetGServerIDByHostID(const int& _hostID)
{
    AutoLock(m_xServerListLock);

    auto lIt = m_umHIDGSIDList.find(_hostID);

    if (lIt == m_umHIDGSIDList.end())
        return 0;

    return lIt->second;
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

void GServerCheckService::FillPacketServerList(flatbuffers::FlatBufferBuilder& _fbb, std::vector<flatbuffers::Offset<DServerInfo>>& _vec, const std::unordered_set<int>& _existPilgrimServerList)
{
    AutoLock(m_xServerListLock);
    for (auto& lIt : m_umGameServerList)
    {
        if (nullptr == lIt.second)
            continue;

        if (lIt.second->m_eServerType != EServer::Game)
            continue;

        auto& lInfo = lIt.second;

        bool lPilgrimExists = false;
        if (_existPilgrimServerList.find(lInfo->m_nServerID) != _existPilgrimServerList.end())
            lPilgrimExists = true;

        _vec.push_back(
            CreateDServerInfo(_fbb
                ,lInfo->m_nServerID
                ,lInfo->m_nServerState
                ,_fbb.CreateString(lInfo->m_sOutboundHost)
                ,lInfo->m_nOutboundPort
                , lPilgrimExists
                ,0)
        );
    }
}

bool GServerCheckService::SetWaitingEnqueue(const int& _serverID, const int& _hostID)
{
    AutoLock(m_xWaitingLock);
    auto lIt = m_umWaitingList.find(_serverID);
    if (lIt == m_umWaitingList.end())
        return false;

    lIt->second.m_vQueueList.emplace_back(_hostID);
    return true;
}

void GServerCheckService::SendPacket(const int& _serverID, const EPacketProtocol& _msgID, void* _msg, const int& _msgSize)
{
    if (nullptr == _msg)
        return;
    
    auto lServer = FindServer(_serverID);

    if (nullptr == lServer)
        return;

    Packet::SharedPtr lPacket = Packet::New();
    if (false == lPacket->SetPacketData(lServer->m_nHostID, _msg, _msgSize))
        return;

    NetworkManager::GetInst().Send(lServer->m_nHostID, lPacket);
}

void GServerCheckService::SendPacket(const int& _serverID, const EPacketProtocol& _msgID, flatbuffers::FlatBufferBuilder& _fbb)
{
    SendPacket(_serverID, _msgID, _fbb.GetBufferPointer(), _fbb.GetSize());
}

int GServerCheckService::GetTotalServerCount() noexcept
{
    return m_nTotalServerCount.load();
}

int GServerCheckService::GetConnectedServerCount() noexcept
{
    return m_nConnectedServerCount.load();
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
        m_umHIDGSIDList.insert_or_assign(_hostID, it->second->m_nServerID);
        it->second->m_bIsConnected = true;
        it->second->m_bIsConnecting = false;

        VIEW_WRITE_INFO("Connected To Game(%d) Server [%s:%d, HostID:%d] Success!"
            , it->second->m_nServerID
            , it->second->m_sOutboundHost.c_str()
            , it->second->m_nOutboundPort
            , _hostID);

        return true;
    }
    return false;
}

bool GServerCheckService::OnHostClose(int _hostID, const HostClose& _msg)
{
    AutoLock(m_xServerListLock);

    auto lServerID = m_umHIDGSIDList.find(_hostID);
    if (lServerID != m_umHIDGSIDList.end())
        m_umHIDGSIDList.erase(lServerID);

    for (auto it = m_umGameServerList.begin(); it != m_umGameServerList.end(); ++it)
    {
        if (nullptr == it->second)
            continue;

        if (it->second->m_nHostID == _hostID)
        {
            if (true == it->second->m_bIsConnected)
            {
                VIEW_WRITE_INFO("%sServer(%d) Disconnected. [%s:%d, HostID: %d]"
                    , (it->second->m_eServerType == EServer::Type::Game) ? "Game" : "Other"
                    , it->second->m_nServerID
                    , it->second->m_sOutboundHost.c_str()
                    , it->second->m_nOutboundPort
                    , _hostID
                );

            }
            else if (true == it->second->m_bIsConnecting)
            {
                VIEW_WRITE_INFO("%sServer(%d) Connecting Failed. [%s:%d, HostID: %d]"
                    , (it->second->m_eServerType == EServer::Type::Game) ? "Game" : "Other"
                    , it->second->m_nServerID
                    , it->second->m_sOutboundHost.c_str()
                    , it->second->m_nOutboundPort
                    , _hostID
                );
            }

            it->second->m_nHostID = 0;
            it->second->m_bIsConnected = false;
            it->second->m_bIsConnecting = false;

            return true;
        }
    }

    return false;
}

void GServerCheckService::_CheckConnect()
{
    AutoLock(m_xServerListLock);

    int lTotalServerCount = static_cast<int>(m_umGameServerList.size());
    int lConnectedServerCount = 0;

    for (auto it = m_umGameServerList.begin(); it != m_umGameServerList.end(); ++it)
    {
        if (nullptr == it->second)
            continue;

        if (false == it->second->m_bIsConnected)
        {
            if (true == NetworkManager::GetInst().IsInitialized())
            {
                if (false == it->second->m_bIsConnecting)
                {
                    bool lConnecting = NetworkManager::GetInst().Connect(it->second->m_oHostEvent.get()
                        , it->second->m_sOutboundHost
                        , it->second->m_nOutboundPort
                        , &it->second->m_nHostID);

                    it->second->m_bIsConnecting = lConnecting;
                }
            }
            else
            {
                ++lConnectedServerCount;
            }
        }

    }

    m_nTotalServerCount.exchange(lTotalServerCount);
    m_nConnectedServerCount.exchange(lConnectedServerCount);

}
