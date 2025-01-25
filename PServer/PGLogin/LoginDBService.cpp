#include "pch.h"
#include "LoginDBService.h"
#include "NetworkCenter.h"

#include <ServerConfig.h>
#include <StringUtil.h>
#include <Random.h>
#include <InnerPacketStructures.h>

LoginDBService::LoginDBService()
{
    m_nCurrentServerID = ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_nServerID;
    RegisterHandler(EPacketProtocol::LUDB_AuthReq, &LoginDBService::_OnLUDBLoginReq);
    RegisterHandler(EPacketProtocol::LUDB_ConnectServerIDClear, &LoginDBService::_OnLUDBConnectServerIDClear);
}

LoginDBService::~LoginDBService()
{
}

bool LoginDBService::_OnLUDBLoginReq(std::shared_ptr<InnerPacket> _data)
{
    CheckSession();

    spLoginAccountProcessSelectDTO* lReq = static_cast<spLoginAccountProcessSelectDTO*>(_data->m_pData);
    int lOTP = (int)Random::GetInst()->GetRandomRange(10000000, 9999999);

    BEGIN_SESSION
        lSess << "{ CALL spAccountLoginProcessSelect(?, ?,?,?,?,?,?, ?,?,?,?,?,?) }"
            , out(lReq->Result)

            , out(lReq->AccountSeq)
            , out(lReq->AccountType)
            , out(lReq->AccountStatus)
            , out(lReq->LastConnectServerID)
            , out(lReq->ConnectedServerID)
            , out(lReq->RemainingPeriod)

            , in(lReq->LoginPlatformType)
            , in(lReq->AccountToken)
            , in(m_nCurrentServerID)
            , in(lOTP)
            , in(lReq->ClientType)
            , in(lReq->AppVersion)
            , in(lReq->BuildType)
            , in(lReq->IPAddress32)

            , now;
    END_SESSION;

    
    //계정 조회 결과에 따른 처리
    switch ((EDBResult)lReq->Result)
    {
        case EDBResult::DuplicateLogin:
            VIEW_DEBUG("DuplicateLogin");
            //중복 로그인일 경우에도 otp 값이 필요하기 때문에 [[fallthrough]] 이용하여
            //다음 case문까지 실행한다.
            [[fallthrough]];
        case EDBResult::Success:
            lReq->OTP = lOTP;
            break;
        case EDBResult::AccountCreateFailed:
        case EDBResult::DurationBlock:  //계정 일시 정지
        case EDBResult::PermanentBlock: //영구 정지
        default:
            break;
    }


    if ((EDBResult)lReq->Result == EDBResult::Success)
    {
        //캐릭터 조회
        std::vector<spLoginAccountPilgrimSelectDTO::AccountPilgrim> lPilgrimList;
        BEGIN_SESSION;

        lSess << "{CALL spAccountPilgrimListSelect(?)}"
            , in(lReq->AccountSeq)
            , into(lPilgrimList)
            , now;

        END_SESSION;

    }

    SendToLoginService(EPacketProtocol::UDBL_AuthRes, _data);

    return true;
}

bool LoginDBService::_OnLUDBConnectServerIDClear(std::shared_ptr<InnerPacket> _data)
{
    if (nullptr == _data.get())
        return false;

    if (nullptr == _data->m_pData)
        return false;

    _ConnectServerIDClear(static_cast<spAccountConnectServerIDClearDTO*>(_data->m_pData)->AccountSeq);

    SafeDelete(_data->m_pData);

    return true;
}

bool LoginDBService::_ConnectServerIDClear(int _serverID)
{
    CheckSession();


    BEGIN_SESSION;
    lSess << "{ CALL spAccountConnectServerIDClear(?) }"
        , in(_serverID)

        , now;
    END_SESSION

    return true;
}
