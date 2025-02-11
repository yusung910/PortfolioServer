#include "pch.h"
#include "OPFService.h"
#include "LoginPlayerManager.h"
#include "NetworkCenter.h"

#include <Base64Util.h>
#include <GlobalEnum.h>
#include <json/json.h>
#include <RSAUtil.h>
#include <ServerConfig.h>
#include <ServerConst.h>
#include <Protocol_generated.h>
#include <NetworkManager.h>

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

        switch (lData->LoginPlatformType)
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
    //https://developer.apple.com/documentation/signinwithapplerestapi/fetch_apple_s_public_key_for_verifying_token_signature#response-codes
    if (nullptr == _data)
        return EF_LOGIN_PF_ERROR;

    m_oHttp.SetContextData(DEFAULT_CTXT_TYPE);
    m_oHttp.SetServerURL("appleid.apple.com");
    m_oHttp.SetSubURL("/auth/keys");
    m_oHttp.SetPost(false);
    m_oHttp.SetServerPort(443);
    m_oHttp.SetSecure(true);

    std::string lParam = "";

    m_oHttp.SetContextData(lParam);

    if (false == m_oHttp.RequestHTTP(lParam))
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_PROTOCOL;

    Json::Reader lReader;
    Json::Value lRoot;

    if (false == lReader.parse(lParam, lRoot))
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_PROTOCOL;

    auto fnStringReplaceAll = [&](std::string& _msg, const std::string& _pattern, const std::string& _replace) -> std::string
        {
            std::string lRslt = _msg;
            std::string::size_type lPos = 0;
            std::string::size_type lOffset = 0;

            while ((lPos = lRslt.find(_pattern, lOffset)) != std::string::npos)
            {
                lRslt.replace(lRslt.begin() + lPos, lRslt.begin() + lPos + _pattern.size(), _replace);

                lOffset = lPos + _replace.size();
            }
            return lRslt;
        };

    auto fnMakeBase64Style = [&](std::string _str) -> void
        {
            while ((int)_str.size() % 4 != 0)
                _str += '=';

            _str = fnStringReplaceAll(_str, "-", "+");
            _str = fnStringReplaceAll(_str, "_", "/");
        };

    //JWT(JSON Web Token) Split
    int lCount = 0;
    std::vector<std::string> lParts;
    std::vector<std::string> lParts2;
    std::istringstream lTokenStream(_data->PlatformToken);
    std::string lStr;

    //getline() : 원하는 구분자(delimiter)를 만날 때 까지 모든 문자열을 입력 받아 하나의 string 객체에 저장
    //apple auth login token example
    //https://developer.apple.com/documentation/signinwithapplerestapi/generate_and_validate_tokens
    //client로부터 받은 token값을 파싱한다
    while (getline(lTokenStream, lStr, '.'))
    {
        lParts2.push_back(lStr);
        fnMakeBase64Style(lStr);

        switch (lCount++)
        {
        case 0:
        case 1:
        {
            Base64Util lBase64;
            char lTmp[2048] = { 0, };
            lBase64.Decode(lStr, lTmp, 2048);

            lParts.push_back(std::string(lTmp));
        }
        break;
        case 2:
            lParts.push_back(lStr);
            break;
        }
    }
    
    //Token값이 없음
    if (lParts.size() < 2 || lParts2.size() < 2)
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_TOKEN;


    // Header
    Json::Value lJsonHeader;
    if(lReader.parse(lParts[0], lJsonHeader) == false)
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_TOKEN;

    // body
    Json::Value lJsonPayload;
    if (lReader.parse(lParts[1], lJsonPayload) == false)
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_TOKEN;

    //Issuer 검증
    if(lJsonPayload["iss"] != "https://appleid.apple.com")
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_TOKEN;

    auto lAppleJWTAudPTR = ServerConst::GetInst().GetString(EServerConst::APPLE_JWT_AUD);

    if (nullptr == lAppleJWTAudPTR)
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_MISSING_KEY;

    auto lAppleJWTAud = StringUtil::WSTR_UTF8(*lAppleJWTAudPTR);
    if (lJsonPayload["aud"] != lAppleJWTAud)
    {
        lAppleJWTAudPTR = ServerConst::GetInst().GetString(EServerConst::APPLE_JWT_AUD2);

        if(nullptr == lAppleJWTAudPTR)
            return EErrorMsg::EF_LOGIN_PF_APPSTORE_MISSING_KEY;

        lAppleJWTAud = StringUtil::WSTR_UTF8(*lAppleJWTAudPTR);

        if(lJsonPayload["aud"] != lAppleJWTAud)
            return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_TOKEN;
    }

    std::string lPfid = lJsonPayload["sub"].asString();
    if (lPfid.empty())
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_ERR_SUBJECT;

    _data->AccountID = lPfid;


    //Signature
    Json::Value lTokenItem;
    for (auto lIt = lRoot["keys"].begin(); lIt != lRoot["keys"].end(); ++lIt)
    {
        lTokenItem = *lIt;
        if (lTokenItem["kid"].asString() == lJsonPayload["kid"].asString()
            && lTokenItem["alg"].asString() == lJsonPayload["alg"].asString())
            break;
    }


    if(lTokenItem.isNull())
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_MISSING_KEY;

    if(lTokenItem["alg"] != "RS256")    // RSA256
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_MISSING_KEY;

    // Modules
    std::string lN = lTokenItem["n"].asString();
    // Exponent
    std::string lE = lTokenItem["e"].asString();

    fnMakeBase64Style(lN);
    fnMakeBase64Style(lE);

    if(true == lN.empty())
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_MISSING_KEY;
    if(true == lE.empty())
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_MISSING_KEY;
    
    std::string lPayload = lParts[0] + "." + lParts2[1];

    if(false == RSAUtil::VerifyJWT(lN, lE, lPayload, lParts2[2]))
        return EErrorMsg::EF_LOGIN_PF_APPSTORE_JWT_ERROR;


    return EErrorMsg::EF_NONE;
}

EErrorMsg OPFService::_AuthProcess_Facebook(LPAuthLogin* _data)
{
    if (nullptr == _data)
        return EF_LOGIN_PF_ERROR;

    return EErrorMsg::EF_NONE;
}

EErrorMsg OPFService::_AuthProcess_Google(LPAuthLogin* _data)
{
    //https://developers.google.com/identity/sign-in/web/backend-auth?hl=ko
    if (nullptr == _data)
        return EF_LOGIN_PF_ERROR;

    m_oHttp.SetContextData(DEFAULT_CTXT_TYPE);
    m_oHttp.SetServerURL("oauth2.googleapis.com");
    m_oHttp.SetSubURL("/tokeninfo");
    m_oHttp.SetPost(false);
    m_oHttp.SetServerPort(443);
    m_oHttp.SetSecure(true);

    std::string lParam = "id_token=" + _data->PlatformToken;

    m_oHttp.SetContextData(lParam);

    std::string lStrRet = "";
    if (m_oHttp.RequestHTTP(lStrRet) == false)
        return EF_LOGIN_PF_GOOGLE_ERR_PROTOCOL;

    Json::Reader lReader;
    Json::Value lRoot;

    if (lReader.parse(lStrRet, lRoot) == false)
        return EF_LOGIN_PF_GOOGLE_ERR_PROTOCOL;

    std::string lPlatformID = lRoot.get("sub", "").asString();

    if (lPlatformID.empty())
        return EF_LOGIN_PF_GOOGLE_ERR_PROTOCOL;

    _data->AccountID = lPlatformID;

    //EF_LOGIN_PF_GOOGLE_EXPIRE

    //EF_LOGIN_PF_GOOGLE_TOKEN



    return EErrorMsg::EF_NONE;
}

EErrorMsg OPFService::_AuthProcess_Naver(LPAuthLogin* _data)
{
    if (nullptr == _data)
        return EF_LOGIN_PF_ERROR;

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
