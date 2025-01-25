/**
 *  @file OPFService.h
 *  @author YS
 *  @date 2025-01-21
 *  @project PGLogin
 *
 *  �ܺ� �÷���(google, naver, facebook, apple ��)�� ����ϱ� ���� Ŭ����
 */
#pragma once
#include <RefSingleton.h>
#include <Service.h>
#include <InnerPacketStructures.h>
#include <Http.h>
class OPFService : public Service, public RefSingleton<OPFService>
{
private:
    Http m_oHttp;

public:
    OPFService();
    virtual ~OPFService();

    bool Start();

protected:
    bool OnLPAuthLoginReq(InnerPacket::SharedPtr _data);

private:
    EErrorMsg _AuthProcess_AppStore(LPAuthLogin* _data);
    EErrorMsg _AuthProcess_Facebook(LPAuthLogin* _data);
    EErrorMsg _AuthProcess_Google(LPAuthLogin* _data);
    EErrorMsg _AuthProcess_Naver(LPAuthLogin* _data);

    bool _SendErrorMessage(const int& _hostID, const EErrorMsg& _errMsg, const EPacketProtocol& _msgID, const bool& isKick = false);

};

