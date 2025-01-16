#include "pch.h"
#include "LoginDBService.h"
#include "NetworkCenter.h"

#include <StringUtil.h>
#include <Random.h>
#include <InnerPacketStructures.h>

LoginDBService::LoginDBService()
{
    RegisterHandler(EPacketProtocol::LUDB_AuthReq, &LoginDBService::OnLUDBLoginReq);
}

LoginDBService::~LoginDBService()
{
}

bool LoginDBService::OnLUDBLoginReq(std::shared_ptr<InnerPacket> _data)
{
    CheckSession();

    LoginAccountProcessSelectDTO* lReq = static_cast<LoginAccountProcessSelectDTO*>(_data->m_pData);
    int lOTP = (int)Random::GetInst()->GetRandomRange(10000000, 9999999);

    BEGIN_SESSION;
        lSess << "{ CALL spAccountLoginProcessSelect(?, ?,?,?,?,?, ?,?,?,?,?,?) }"
            , out(lReq->Result)

            , out(lReq->AccountSeq)
            , out(lReq->AccountType)
            , out(lReq->AccountStatus)
            , out(lReq->LastConnectGameServerID)
            , out(lReq->DeleteRemainingPeriod)

            , in(lReq->LoginPlatformType)
            , in(lReq->AccountUIDkey)
            , in(lReq->ConnectingLoginServerID)
            , in(lOTP)
            , in(lReq->ClientType)
            , in(lReq->AppVersion)
            , in(lReq->BuildType)
            , in(lReq->IPAddress32)

            ,now;
    END_SESSION
    
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
        std::vector<LoginAccountPilgrimSelectDTO::AccountPilgrim> lPilgrimList;
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
