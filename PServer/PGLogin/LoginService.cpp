#include "pch.h"
#include "LoginService.h"
#include "StringUtil.h"
#include "Clock.h"
#include "ServerConfig.h"
#include "RevUtil.h"
#include "LoginPlayerManager.h"
#include "StrChecker.h"
#include "NetworkCenter.h"
#include "GServerCheckService.h"

#include <GlobalConst.h>
#include <InnerPacketStructures.h>
#include <NetworkManager.h>
#include <NetworkStatistics.h>
#include <PFunc.h>
#include <PConstVars.h>
#include <PocoTimeUtil.h>
#include <ServerMonitor.h>
#include <Timer.h>

LoginService::LoginService()
{
    RegisterHandler(&LoginService::OnHostConnect);
    RegisterHandler(&LoginService::OnHostClose);
    RegisterHandler(&LoginService::OnCLAuthReq);
    RegisterInnerHandler(EPacketProtocol::UDBL_AuthRes, &LoginService::OnUDBLAuthRes);
    RegisterInnerHandler(EPacketProtocol::PL_AuthLoginRes, &LoginService::OnPLAuthLoginRes);

}

LoginService::~LoginService()
{
}

bool LoginService::Start()
{
    RegisterTimer(LOGIN_SERVICE_KICK_DELAY_MS, std::bind(&LoginService::_KickProcess, this));
    RegisterTimer(PG_TRAFFIC_CHECK_TIME_MS, std::bind(&LoginService::_Statistics, this));
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
    VIEW_INFO(L"Host(%d) Connected::(%s:%d)", _hostID, StringUtil::ToWideChar(_msg.peerip()->data()).c_str(), _msg.peerport());
    NetworkManager::GetInst().OnConnect(_hostID);
    NetworkManager::GetInst().SetClientHostMode(_hostID, true);

    return true;
}

bool LoginService::OnHostClose(int _hostID, const HostClose& _msg)
{
    VIEW_INFO(L"Host(%d) Disconnected.", _hostID);
    NetworkManager::GetInst().OnDisconnect(_hostID);
    return true;
}

bool LoginService::OnCLAuthReq(int _hostID, const CLAuthReq& _msg)
{
    auto lPlayer = LoginPlayerManager::GetInst().Add(_hostID, _msg);

    if (nullptr == lPlayer)
    {
        VIEW_WRITE_ERROR(L"OnCLAuthReq :: %d Duplicate Auth Request!!", _hostID);
        _SendErrorMessage(_hostID, EErrorMsg::EF_KICK_DUPLICATE_LOGIN, _msg.messageid());
        AddKickReserve(_hostID);

        return false;
    }

    // Client ����
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

    //��Ŷ�� AppVersion�� Ŭ���̾�Ʈ Ÿ���� ���Ѵ�
    //Ŭ���̾�Ʈ�� invalid�� �����ϱ� ���Ѱ����� ���δ�.
    if (lClientVer != 0
        && lClientVer > _msg.AppVersion())
    {
        VIEW_WRITE_ERROR(L"OnCLAuthReq - Version is Not Machted ClientType: %d", lClientType);
        return _SendErrorMessage(_hostID, EErrorMsg::EF_KICK_INVALID_APP_VERSION, _msg.messageid(), true);
    }

    // �÷��� Ȯ��
    if (PFunc::GetInst().CheckHasStr(_msg.AccountToken()) == false)
    {
        VIEW_WRITE_ERROR(L"OnCLAuthReq :: %d UniqueKey is Missing", _hostID);
        //�����޼��� ����
        return _SendErrorMessage(_hostID, EErrorMsg::EF_NONE, _msg.messageid(), true);
    }

    std::string lAccountToken = (nullptr == _msg.AccountToken()) ? "" : _msg.AccountToken()->c_str();

    
    if(ELoginPlatform::IsGuestPlatform((ELoginPlatform::Type)_msg.LoginPlatformType()))
    {
        //�α��� �÷��� Ÿ���� Guest�� ��
        bool lIsValidKey = StrChecker::GetInst().IsValidStrAccountToken(lAccountToken, 0, ACCOUNT_UNIQUE_KEY_MAXSIZE);

        if (false == lIsValidKey)
        {
            VIEW_WRITE_ERROR(L"LoginService :: OnCLAuthReq AccountToken is Invalid");
            _SendErrorMessage(_hostID, EErrorMsg::EF_LOGIN_ACCOUNT_UNIQUE_KEY_INVALID, _msg.messageid(), true);
            AddKickReserve(_hostID);
            return false;
        }

        _AuthLoginProcess(_hostID, _msg.ClientType(), _msg.AppVersion(), (ELoginPlatform::Type)_msg.LoginPlatformType(), lAccountToken);
    }
    else
    {
        lPlayer->m_eState = ELoginState::PlatformAuthorize;
        LPAuthLogin* lLPAuth = new LPAuthLogin;
        lLPAuth->LoginPlatformType = (ELoginPlatform::Type)_msg.LoginPlatformType();
        lLPAuth->AccountToken = lAccountToken;

        lLPAuth->ClientType = _msg.ClientType();
        lLPAuth->AppVersion = _msg.AppVersion();

        SendToPlatform(_hostID, EPacketProtocol::LP_AuthLoginReq, lLPAuth);

    }
    

    return true;
}

bool LoginService::OnUDBLAuthRes(InnerPacket::SharedPtr _data)
{
    if (nullptr == _data.get())
        return false;
    if (nullptr == _data->m_pData)
        return false;

    spLoginAccountProcessSelectDTO* lRes = static_cast<spLoginAccountProcessSelectDTO*>(_data->m_pData);

    //
    auto lPc = LoginPlayerManager::GetInst().Find(_data->m_nHostID);
    if (nullptr == lPc)
    {
        VIEW_WRITE_ERROR(L"OnUDBLAuthRes :: HostID(%d) is Missing", _data->m_nHostID);
        AddKickReserve(_data->m_nHostID);
        return false;
    }


    //���� �����Ǿ� �ִ� ����
    if (lRes->Result == (int)EDBResult::PermanentBlock)
    {
        //_SendErrorMessage(EF_)
        AddKickReserve(_data->m_nHostID);
        return false;
    }

    //�Ⱓ ����
    if (lRes->Result == (int)EDBResult::DurationBlock)
    {
        //_SendErrorMessage(EF_)
        AddKickReserve(_data->m_nHostID);
        return false;
    }

    //AccountSeq ���
    lPc->m_nAccountSeq = lRes->AccountSeq;

    if (lRes->RemainingPeriod != Poco::DateTime(1900, 1, 1))
    {
        //���� or �Ͻ� ���� ĳ�� - ���� �������ͽ� ���¿� ���� ���� or �Ͻ����� �Ⱓ�� �����ش�
        flatbuffers::FlatBufferBuilder lFbb;
        const auto& diff = lRes->RemainingPeriod - PocoTimeUtil::GetLocalTime();
        auto lPacket = CreateLCAuthErrorRes(lFbb, EErrorMsg::EF_SANCTION_ACCOUNT);
        lFbb.Finish(lPacket);

        VIEW_WRITE_ERROR(L"OnUDBLAuthRes :: Sanction Account, HostID: %d, AccountSeq: %d", lPc->m_nHostID, lRes->AccountSeq);
        LoginPlayerManager::GetInst().SendPacket(_data->m_nHostID, EPacketProtocol::LC_AuthErrorRes, lFbb);
        return false;
    }

    int lLastGameServer = lRes->LastConnectServerID;
    if (nullptr == GServerCheckService::GetInst().FindServer(lLastGameServer))
        lLastGameServer = GServerCheckService::GetInst().GetLatestGameServerID();

    lPc->m_nSelectedServerID = lLastGameServer;
    lPc->m_nOTP = lRes->OTP;

    //�÷��̾� ĳ���� seq ����
    lPc->m_umPilgrimSeqList.swap(lRes->PilgrimExistServerList);
    if (lRes->Result == (int)EDBResult::DuplicateLogin)
    {
        //���������� ������ gameServerID
        auto lServerInfo = GServerCheckService::GetInst().FindServer(lRes->LastConnectServerID);
        
        if(nullptr != lServerInfo
            && true == lServerInfo->m_bIsConnected)
        {
            flatbuffers::FlatBufferBuilder lFbb;

            auto lPacket = CreateLSKickDuplicateConnectUserReq(lFbb, lRes->AccountSeq, EF_KICK_DUPLICATE_LOGIN, lRes->LastConnectServerID);
            lFbb.Finish(lPacket);

            GServerCheckService::GetInst().SendPacket(lRes->LastConnectServerID, EPacketProtocol::LS_KickDuplicateConnectUserReq, lFbb);

            lPc->m_eState = ELoginState::DuplicateKick;

            VIEW_WRITE_ERROR(L"OnUDBLAuthRes - HostID: %d, Duplicate Login. AccountSeq: %d", _data->m_nHostID, lRes->AccountSeq);

            return true;
        }
        else
        {
            if (true == ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_bIsLive)
            {
                VIEW_WRITE_ERROR(L"OnUDBLAuthRes - HostID: %d, Duplicate Login. AccountSeq: %d", _data->m_nHostID, lRes->AccountSeq);

                //live ����
                spAccountConnectServerIDClearDTO* lReq = new spAccountConnectServerIDClearDTO;
                lReq->AccountSeq = (int)lRes->AccountSeq;
                SendToUDB(_data->m_nHostID, EPacketProtocol::LUDB_ConnectServerIDClear, lReq);

                return _SendErrorMessage(_data->m_nHostID, EErrorMsg::EF_LOGIN_ERROR, EPacketProtocol::CS_AuthReq, true);
            }
            else
            {
                if(lRes->ConnectedServerID < 10901
                    || lRes->ConnectedServerID > 10999)
                    return _SendErrorMessage(_data->m_nHostID, EErrorMsg::EF_KICK, EPacketProtocol::CS_AuthReq, true);
            }

            spAccountConnectServerIDClearDTO* lReq = new spAccountConnectServerIDClearDTO;
            lReq->AccountSeq = (int)lRes->AccountSeq;
            SendToUDB(_data->m_nHostID, EPacketProtocol::LUDB_ConnectServerIDClear, lReq);


            return _SendErrorMessage(_data->m_nHostID, EErrorMsg::EF_LOGIN_ERROR, EPacketProtocol::CS_AuthReq, true);
        }


        return false;
    }

    flatbuffers::FlatBufferBuilder lFbb;
    std::vector<flatbuffers::Offset<DServerInfo>> lServerList;
    GServerCheckService::GetInst().FillPacketServerList(lFbb, lServerList, lPc->m_umPilgrimSeqList);
    auto lPacket = CreateLCAuthRes(lFbb, lRes->AccountSeq, lFbb.CreateVector(lServerList), lRes->LastConnectServerID, Clock::GetTick64(), PocoTimeUtil::GetLocalTimezone());

    lFbb.Finish(lPacket);

    LoginPlayerManager::GetInst().SendPacket(_data->m_nHostID, EPacketProtocol::LC_AuthRes, lFbb);
    //


    return true;
}

bool LoginService::OnPLAuthLoginRes(InnerPacket::SharedPtr _data)
{

    return true;
}

bool LoginService::_SendErrorMessage(const int& _hostID, const EErrorMsg& _errorMsg, const EPacketProtocol& _msgID, const bool& _kick)
{
    flatbuffers::FlatBufferBuilder lFbb;
    auto lPacket = CreateSCIntegrationErrorNotification(lFbb, _msgID, (int)_errorMsg);
    lFbb.Finish(lPacket);

    LoginPlayerManager::GetInst().SendPacket(_hostID, EPacketProtocol::SC_IntegrationErrorNotification, lFbb);

    if (true == _kick)
    {
        AddKickReserve(_hostID);
    }

    return false;
}

bool LoginService::_AuthLoginProcess(int _hostID, const int& _clientType, const int& _appVer, const ELoginPlatform::Type _pfType, const std::string& _accountToken)
{
    auto lPc = LoginPlayerManager::GetInst().Find(_hostID);

    if (nullptr == lPc)
    {
        VIEW_WRITE_ERROR(L"_AuthLoginProcess :: pc is Missing");
        _SendErrorMessage(_hostID, EErrorMsg::EF_LOGIN_ACCOUNT_UNIQUE_KEY_INVALID, EPacketProtocol::CL_AuthReq, true);
        AddKickReserve(_hostID);
        return false;
    }

    if (true == _accountToken.empty())
    {
        VIEW_WRITE_ERROR(L"_AuthLoginProcess :: _accountToken is Missing");

        return _SendErrorMessage(_hostID, EErrorMsg::EF_FAIL_MISSING_REQUIRED_FIELD, EPacketProtocol::CL_AuthReq, true);
    }

    lPc->m_eState = ELoginState::DBProcess;

    spLoginAccountProcessSelectDTO* lDTO = new spLoginAccountProcessSelectDTO();
    lDTO->ClientType = _clientType;
    lDTO->AppVersion = _appVer;
    lDTO->AccountToken = _accountToken;
    lDTO->LoginPlatformType = _pfType;
    lDTO->IPAddress32 = NetworkManager::GetInst().GetIPInt32(_hostID);

    SendToUDB(_hostID, EPacketProtocol::LUDB_AuthReq, lDTO);

    return true;
}

void LoginService::_KickProcess()
{
    int64_t lNow = Clock::GetTick64();

    AutoLock(m_xKickLock);

    for (auto it = m_umKickList.begin(); it != m_umKickList.end();)
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

    int lTotalServerCount = GServerCheckService::GetInst().GetTotalServerCount();

    int lConnectedServerCount = GServerCheckService::GetInst().GetConnectedServerCount();

    sprintf_s(lTmp, sizeof(lTmp), "TITLE LoginServer(%d) Rev: %d(%d), Connect : [%d/%d]",
        lServerID,
        lVer,
        lLibVer,
        lTotalServerCount,
        lConnectedServerCount
    );

    system(lTmp);

}