#include "pch.h"
#include "LoginService.h"
#include "StringUtil.h"
#include "Clock.h"
#include "ServerConfig.h"
#include "RevUtil.h"
#include "LoginPlayerManager.h"

#include <NetworkManager.h>
#include <NetworkStatistics.h>
#include <PublicFunc.h>
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
    auto lTick = Clock::GetTick64() + KICK_RESERVE_DELAY_MS;
    m_umKickList.emplace(_hostID, lTick);
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

bool LoginService::OnCLAuthReq(int _hostID, const CLAuthReq& _msg)
{
    auto lPlayer = LoginPlayerManager::GetInst().Add(_hostID, _msg);
    if (nullptr == lPlayer)
    {
        VIEW_WRITE_ERROR(L"OnCLAuthReq :: %d Duplicate Auth Request!!", _hostID);
        _SendErrorMessage(_hostID, EErrorMsg::EF_KICK_DUPLICATE_LOGIN, _msg.messageid(), true);
        AddKickReserve(_hostID);

        return false;
    }

    // Client 타입 확인
    int lClientVer = 0;

    const auto lClientType = _msg.ClientType();
    switch (static_cast<EClient::Type>(lClientType))
    {
    case EClient::Windows:
        break;
    case EClient::Android:
        break;
    case EClient::iOS:
        break;
    case EClient::WindowsUniversal:
        break;
    default:
        VIEW_WRITE_ERROR(L"OnCLAuthReq :: Invalid ClientType:%d", lClientType);
        return false;
    }
    
    // 플랫폼 확인
    if (PublicFunc::GetInst().CheckHasStr(_msg.UniqueKey()) == false)
    {
        VIEW_WRITE_ERROR(L"OnCLAuthReq :: %d UniqueKey is Missing", _hostID);
        //에러메세지 전송
        return false;
    }

    return true;
}

void LoginService::_SendErrorMessage(const int& _hostID, const EErrorMsg& _errorMsg, const EPacketProtocol& _msgID, const bool& _kick)
{
    flatbuffers::FlatBufferBuilder lFbb;
    auto lPacket = CreateSCIntegrationErrorNotification(lFbb, _msgID, (int)_errorMsg);

    LoginPlayerManager::GetInst().SendPacket(_hostID, EPacketProtocol::SC_IntegrationErrorNotification, lFbb);

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
        ++it;
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
