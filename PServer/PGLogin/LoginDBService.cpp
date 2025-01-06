#include "pch.h"
#include "LoginDBService.h"
#include "NetworkCenter.h"

#include <StringUtil.h>
#include <Random.h>
#include <InnerPacketStructures.h>

LoginDBService::LoginDBService()
{
    RegisterHandler(EPacketProtocol::LUDB_AuthReq, &LoginDBService::OnLULoginReq);
}

LoginDBService::~LoginDBService()
{
}

bool LoginDBService::OnLULoginReq(std::shared_ptr<InnerPacket> _data)
{
    CheckSession();

    pLoginAccountResult* lReq = static_cast<pLoginAccountResult*>(_data->m_pData);
    int lOTP = (int)Random::GetInst()->GetRandomRange(10000000, 9999999);

    try
    {
        //lSess << "{ CALL  ~() }"
        //    , out()
        //    ,
        // now;
    }
    catch (Poco::Data::ODBC::StatementException& lEx)
    {
        VIEW_WRITE_ERROR(L"\n%s", StringUtil::UTF8_WSTR(lEx.message().c_str()).c_str());
    }
    catch (std::exception& lE)
    {
        VIEW_WRITE_ERROR(L"\nDB Error %s", lE.what());
    }
    
    switch ((EDBResult)lReq->Result)
    {
        case EDBResult::Success:
            lReq->OTP = lOTP;
            break;

        default:
            break;
    }


    if ((EDBResult)lReq->Result == EDBResult::Success)
    {
        //캐릭터 조회

    }

    SendToLoginService(EPacketProtocol::UDBL_AuthRes, _data);

    return true;
}
