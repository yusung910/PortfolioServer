#pragma once
#include <PGObject.h>
#include <GlobalEnum.h>
#include <unordered_set>

#include "ELoginState.h"

class LoginPlayer : public PGObject<LoginPlayer>
{
public:
    EClient::Type m_eClientType = EClient::Type::None;  //���� Ŭ�� ����
    EBuild::Type m_eBuildType = EBuild::Type::None;     //���� ����
    ELoginPlatform::Type m_eLoginPlatformType = ELoginPlatform::Type::None;     //���� ����� Ÿ��

    int m_nAppVersion = 0;                              //Ŭ�� �� ����
    int m_nBuildStamp = 0;                              //Ŭ�� ���� ��ȣ

    std::string m_sAuthKey = "";                        //key
    int m_nAccountSeq = 0;                              //���� ���� ������
    int m_nSelectedServerID = 0;                        //������ ����

    bool m_bIsAllowed = false;                          //���� ��� ����

    ELoginState m_eState = ELoginState::None;           //�α��� ���� ����
    int m_nHostID = 0;                                  //

    std::string m_sReturnToken = "";                    //
    std::string m_sPlatformID = "";                     //

    int m_nOTP = 0;                                     //

    std::unordered_set<int> m_usPilgrimSeqList;        //������ �����ϴ� ĳ���� ���� ������ ���

public:
    LoginPlayer(ObjectTag);
    virtual ~LoginPlayer() = default;

    virtual void Reset();

};

