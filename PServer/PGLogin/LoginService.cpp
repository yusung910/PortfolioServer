#include "pch.h"
#include "LoginService.h"
#include "StringUtil.h"
#include "Clock.h"
#include "ServerConfig.h"
#include "RevUtil.h"
#include "LoginPlayerManager.h"
#include "StrChecker.h"
#include "NetworkCenter.h"

#include <NetworkManager.h>
#include <NetworkStatistics.h>
#include <PFunc.h>
#include <PConstVars.h>
#include <ServerMonitor.h>
#include <Timer.h>
#include <GlobalConst.h>
#include <InnerPacketStructures.h>

LoginService::LoginService()
{
    RegisterHandler(&LoginService::OnHostConnect);
    RegisterHandler(&LoginService::OnHostClose);
    RegisterHandler(&LoginService::OnCLAuthReq);
    RegisterInnerHandler(EPacketProtocol::UDBL_AuthRes, &LoginService::OnUDBLAuthRes);

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

    // Client 버전
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

    //패킷의 AppVersion과 클라이언트 타입을 비교한다
    //클라이언트의 invalid를 검증하기 위한것으로 보인다.
    if (lClientVer != 0
        && lClientVer > _msg.AppVersion())
    {
        VIEW_WRITE_ERROR(L"OnCLAuthReq - Version is Not Machted ClientType: %d", lClientType);
        return _SendErrorMessage(_hostID, EErrorMsg::EF_KICK_INVALID_APP_VERSION, _msg.messageid(), true);
    }

    // 플랫폼 확인
    if (PFunc::GetInst().CheckHasStr(_msg.AccountUKey()) == false)
    {
        VIEW_WRITE_ERROR(L"OnCLAuthReq :: %d UniqueKey is Missing", _hostID);
        //에러메세지 전송
        return _SendErrorMessage(_hostID, EErrorMsg::EF_NONE, _msg.messageid(), true);
    }


    std::string lAccountUKey = (nullptr == _msg.AccountUKey()) ? "" : _msg.AccountUKey()->c_str();

    
    //로그인 플랫폼 타입(Google, AppStore 등등)에 따른 처리
    switch ((ELoginPlatform::Type)_msg.LoginPlatformType())
    {
        case ELoginPlatform::GuestLogin:
        {
            bool lIsValidKey = StrChecker::GetInst().IsValidStr(lAccountUKey, 0, ACCOUNT_UNIQUE_KEY_MAXSIZE);

            if (false == lIsValidKey)
            {
                VIEW_WRITE_ERROR(L"LoginService :: OnCLAuthReq AccountUKey is Invalid");
                _SendErrorMessage(_hostID, EErrorMsg::EF_LOGIN_ACCOUNT_UNIQUE_KEY_INVALID, _msg.messageid(), true);
                AddKickReserve(_hostID);
                return false;
            }

        }
        break;

    }

    return true;
}

bool LoginService::OnUDBLAuthRes(InnerPacket::SharedPtr _data)
{
    if (nullptr == _data.get())
        return false;
    if (nullptr == _data->m_pData)
        return false;

    LoginAccountProcessSelectDTO* lRes = static_cast<LoginAccountProcessSelectDTO*>(_data->m_pData);

    //
    auto lPc = LoginPlayerManager::GetInst().Find(_data->m_nHostID);
    if (nullptr == lPc)
    {
        VIEW_WRITE_ERROR(L"OnUDBLAuthRes :: HostID(%d) is Missing", _data->m_nHostID);
        AddKickReserve(_data->m_nHostID);
        return false;
    }


    //영구 제제되어 있는 계정
    if (lRes->Result == (int)EDBResult::PermanentBlock)
    {
        //_SendErrorMessage(EF_)
        AddKickReserve(_data->m_nHostID);
        return false;
    }

    //기간 제제
    if (lRes->Result == (int)EDBResult::DurationBlock)
    {
        //_SendErrorMessage(EF_)
        AddKickReserve(_data->m_nHostID);
        return false;
    }

    //AccountSeq 기록
    lPc->m_nAccountSeq = lRes->AccountSeq;

    






    return true;
}

bool LoginService::_SendErrorMessage(const int& _hostID, const EErrorMsg& _errorMsg, const EPacketProtocol& _msgID, const bool& _kick)
{
    flatbuffers::FlatBufferBuilder lFbb;
    auto lPacket = CreateSCIntegrationErrorNotification(lFbb, _msgID, (int)_errorMsg);

    LoginPlayerManager::GetInst().SendPacket(_hostID, EPacketProtocol::SC_IntegrationErrorNotification, lFbb);

    if (true == _kick)
    {
        AddKickReserve(_hostID);
    }

    return false;
}

bool LoginService::_AuthLoginProcess(int _hostID, const int& _clientType, const int& _appVer, const ELoginPlatform::Type _pfType, const std::string& _accountUKey)
{
    auto lPc = LoginPlayerManager::GetInst().Find(_hostID);

    if (nullptr == lPc)
    {
        VIEW_WRITE_ERROR(L"_AuthLoginProcess :: pc is Missing");
        _SendErrorMessage(_hostID, EErrorMsg::EF_LOGIN_ACCOUNT_UNIQUE_KEY_INVALID, EPacketProtocol::CL_AuthReq, true);
        AddKickReserve(_hostID);
        return false;
    }

    if (true == _accountUKey.empty())
    {
        VIEW_WRITE_ERROR(L"_AuthLoginProcess :: _accountUKey is Missing");

        return _SendErrorMessage(_hostID, EErrorMsg::EF_FAIL_MISSING_REQUIRED_FIELD, EPacketProtocol::CL_AuthReq, true);
    }

    lPc->m_eState = ELoginState::DBProcess;

    LoginAccountProcessSelectDTO* ldto = new LoginAccountProcessSelectDTO();
    ldto->ClientType = _clientType;
    ldto->AppVersion = _appVer;
    ldto->AccountUIDkey = _accountUKey;
    ldto->LoginPlatformType = _pfType;
    ldto->IPAddress32 = NetworkManager::GetInst().GetIPInt32(_hostID);

    SendToUDB(_hostID, EPacketProtocol::LUDB_AuthReq, ldto);

    return true;
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

bool LoginService::_AuthGuestLoginProcess(int _hostID, const int& _clientType, const int& _appVer, const ELoginPlatform::Type& _type, const std::string& _accountUKey)
{
    VIEW_INFO("OnCLAuthReq:: _AuthGuestLoginProcess HostID: %d", _hostID);

    auto lPc = LoginPlayerManager::GetInst().Find(_hostID);

    if (nullptr == lPc)
    {

        return false;
    }

    if (true == _accountUKey.empty())
    {
        //클라에서 AccountUkey가 없을 경우
        return false;
    }

    return true;
}
