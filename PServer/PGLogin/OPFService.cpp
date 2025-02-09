#include "pch.h"
#include "OPFService.h"
#include "LoginPlayerManager.h"
#include "NetworkCenter.h"

#include <json/json.h>
#include <Base64Util.h>

//#include <RSAUtil.h>

#include <NetworkManager.h>
#include <GlobalEnum.h>
//jwt
#pragma warning (push)
#pragma warning (disable:4267)
//https://github.com/Thalhammer/jwt-cpp
#include <jwt-cpp/jwt.h>
#pragma warning (pop)

#pragma comment (lib, "libcrypto.lib")
#pragma comment (lib, "libssl.lib")

// end, jwt and ssl

OPFService::OPFService()
{
    RegisterInnerHandler(EPacketProtocol::LP_AuthLoginReq, &OPFService::OnLPAuthLoginReq);
}

OPFService::~OPFService()
{
}

bool OPFService::Start()
{
    return CreateThread();
}

bool OPFService::OnLPAuthLoginReq(InnerPacket::SharedPtr _data)
{
    if (nullptr == _data.get())
        return false;

    if (nullptr == _data->m_pData)
        return false;

    try
    {
        //Client Other Platform Token invalid Check
        LPAuthLogin* lData = static_cast<LPAuthLogin*>(_data->m_pData);

        switch(lData->LoginPlatformType)
        {
        case ELoginPlatform::AppStore:
            lData->Result = _AuthProcess_AppStore(lData);
            break;
        case ELoginPlatform::Facebook:
            lData->Result = _AuthProcess_Facebook(lData);
            break;
        case ELoginPlatform::Google:
            lData->Result = _AuthProcess_Google(lData);
            break;
        case ELoginPlatform::Naver:
            lData->Result = _AuthProcess_Naver(lData);
            break;
        default:
            break;
        }
    }
    catch (std::exception& _e)
    {
        VIEW_WRITE_ERROR("OnLPAuthLoginReq :: Error - %s", _e.what());
    }


    SendToLoginService(EPacketProtocol::PL_AuthLoginRes, _data);

    return true;
}

EErrorMsg OPFService::_AuthProcess_AppStore(LPAuthLogin* _data)
{
    if (nullptr == _data)
        return EF_LOGIN_ERROR;

    return EErrorMsg::EF_NONE;
}

EErrorMsg OPFService::_AuthProcess_Facebook(LPAuthLogin* _data)
{
    if (nullptr == _data)
        return EF_LOGIN_ERROR;

    return EErrorMsg::EF_NONE;
}

EErrorMsg OPFService::_AuthProcess_Google(LPAuthLogin* _data)
{
    if (nullptr == _data)
        return EF_LOGIN_ERROR;

    m_oHttp.SetContextData(DEFAULT_CTXT_TYPE);
    m_oHttp.SetServerURL("oauth2.googleapis.com");
    m_oHttp.SetSubURL("/tokeninfo");
    m_oHttp.SetPost(false);
    m_oHttp.SetServerPort(443);
    m_oHttp.SetSecure(true);

    std::string lParam = "id_token=" + _data->AccountToken;

    m_oHttp.SetContextData(lParam);
    
    std::string lStrRet = "";
    if (m_oHttp.RequestHTTP(lStrRet) == false)
        return EF_LOGIN_PF_GOOGLE_ERROR;

    Json::Reader lReader;
    Json::Value lRoot;

    if(lReader.parse(lStrRet, lRoot) == false)


    return EErrorMsg::EF_NONE;
}

EErrorMsg OPFService::_AuthProcess_Naver(LPAuthLogin* _data)
{
    if (nullptr == _data)
        return EF_LOGIN_ERROR;

    return EErrorMsg::EF_NONE;
}

bool OPFService::_SendErrorMessage(const int& _hostID, const EErrorMsg& _errMsg, const EPacketProtocol& _msgID, const bool& isKick)
{
    flatbuffers::FlatBufferBuilder lFbb;
    auto lPacket = CreateSCIntegrationErrorNotification(lFbb, _msgID, (int)_errMsg);

    lFbb.Finish(lPacket);

    LoginPlayerManager::GetInst().SendPacket(_hostID, EPacketProtocol::SC_IntegrationErrorNotification, lFbb);

    if (true == isKick)
        NetworkManager::GetInst().Close(_hostID);

    return false;
}
