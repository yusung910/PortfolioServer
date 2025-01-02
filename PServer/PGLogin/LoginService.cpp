#include "pch.h"
#include "LoginService.h"
#include "StringUtil.h"
#include "Clock.h"
#include "ServerConfig.h"
#include "RevUtil.h"

#include <NetworkManager.h>
#include <NetworkStatistics.h>
#include <PGConstVars.h>
#include <ServerMonitor.h>
#include <Timer.h>

LoginService::LoginService()
{
    RegisterHandler(&LoginService::OnHostConnect);
    RegisterHandler(&LoginService::OnHostClose);
}

LoginService::~LoginService()
{
}

bool LoginService::Start()
{
    RegisterTimer(LOGIN_SERVICE_KICK_DELAY_MS, std::bind(&LoginService::_KickProcess, this));
    RegisterTimer(KICK_RESERVE_DELAY_MS, std::bind(&LoginService::_Statistics, this));
    RegisterTimer(5000, std::bind(&LoginService::_UpdateTitle, this));

    return CreateThread();
}

void LoginService::AddKickReserve(const int& _hostID)
{
    AutoLock(m_xKickLock);
}

bool LoginService::OnHostConnect(int _hostID, const HostConnect& _msg)
{
    VIEW_DEBUG(L"Host(%d) Connected::(%s : %d)", _hostID, StringUtil::ToWideChar(_msg.peerip()->data()).c_str(), _msg.peerport());
    NetworkManager::GetInst().OnConnect(_hostID);
    NetworkManager::GetInst().SetClientHostMode(_hostID, true);

    return true;
}

bool LoginService::OnHostClose(int _hostID, const HostConnect& _msg)
{
    VIEW_DEBUG(L"Host(%d) Disconnected.", _hostID);
    NetworkManager::GetInst().OnDisconnect(_hostID);
    return true;
}

void LoginService::_SendErrorMessage(const int& _hostID, const EErrorMsg& _errorMsg, const EPacketProtocol& _msgID, const bool& _kick)
{
    flatbuffers::FlatBufferBuilder lFbb;
    auto lPacket = CreateSCIntegrationErrorNotification(lFbb, _msgID, (int)_errorMsg);

    if (true == _kick)
    {
        AddKickReserve(_hostID);
    }
}

void LoginService::_KickProcess()
{
    int64_t lNow = Clock::GetTick64();

    AutoLock(m_xKickLock);

    for (auto it = m_umKickList.begin(); it != m_umKickList.end(); ++it)
    {
        if (lNow > it->second)
        {
            NetworkManager::GetInst().CloseHost(it->first, "");

            it = m_umKickList.erase(it);
            continue;
        }

    }

}

void LoginService::_Statistics()
{
    auto lStat = NetworkManager::GetInst().GetStatistics();

    if (nullptr == lStat)
        return;

    int lSecn = PG_TRAFFIC_CHECK_TIME_MS / 1000;
    float lSecf = 1000.0f / PG_TRAFFIC_CHECK_TIME_MS;

    auto lRecvBPS = lStat->RecvBytes() / lSecn;
    float lRecvCPSf = (float)(lStat->RecvCount() * lSecf);

    auto lSendBPS = lStat->SendBytes() / lSecn;
    float lSendCPS = (float)(lStat->SendCount() * lSecf);

    auto lConn = lStat->CurrentConnection();
    auto lTotalConn = lStat->TotalConnection();

    std::string lMem = ServerMonitor::GetInst().GetCurrentMemoryString();
    std::string lCPU = ServerMonitor::GetInst().GetCurrentCPUUsageString();

    auto _GetBPStr = [](int64_t _bytes) -> std::string
        {
            char lTmp[MAX_PATH] = { 0, };

            if (_bytes <= 1024)
            {
                sprintf_s(lTmp, sizeof(lTmp), "%d Bps", (int)_bytes);
            }
            else if (_bytes <= 1024 * 1024)
            {
                float lBytes = _bytes / 1024.0f;
                sprintf_s(lTmp, sizeof(lTmp), "%.2f KBps", lBytes);
            }
            else if (_bytes <= 1024 * 1024 * 1024)
            {
                float lBytes = _bytes / 1024.0f / 1024.0f;
                sprintf_s(lTmp, sizeof(lTmp), "%.2f MBps", lBytes);
            }

            return std::string(lTmp);
        };

    VIEW_WRITE_INFO("NetStat Send[%s %.1f pps] Recv[%s %.1f pps] Conn(%d/%d) (Memory: %s) (CPU : %s) ", _GetBPStr(lSendBPS).c_str(), lSendCPS, _GetBPStr(lRecvBPS).c_str(), lRecvCPSf, lConn, lTotalConn, lMem, lCPU);
}

void LoginService::_UpdateTitle()
{
    char lTmp[MAX_PATH] = { 0, };
    int lServerID = ServerConfig::GetInst().GetConfig().GetServerID();

    int lVer = GetServerVer();
    int lLibVer = GetLibraryRev();


    system(lTmp);
}
