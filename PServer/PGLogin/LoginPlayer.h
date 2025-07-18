#pragma once
#include <PGObject.h>
#include <GlobalEnum.h>
#include <unordered_set>

#include "ELoginState.h"

class LoginPlayer : public PGObject<LoginPlayer>
{
public:
    EClient::Type m_eClientType = EClient::Type::None;  //접속 클라 유형
    EBuild::Type m_eBuildType = EBuild::Type::None;     //빌드 종류
    ELoginPlatform::Type m_eLoginPlatformType = ELoginPlatform::Type::None;     //결제 스토어 타입

    int m_nAppVersion = 0;                              //클라 앱 버전
    int m_nBuildStamp = 0;                              //클라 빌드 번호

    std::string m_sAuthKey = "";                        //key
    int m_nAccountSeq = 0;                              //계정 고유 시퀀스
    int m_nSelectedServerID = 0;                        //선택한 서버

    bool m_bIsAllowed = false;                          //접속 허용 여부

    ELoginState m_eState = ELoginState::None;           //로그인 절차 상태
    int m_nHostID = 0;                                  //

    std::string m_sReturnToken = "";                    //
    std::string m_sPlatformID = "";                     //

    int m_nOTP = 0;                                     //

    std::unordered_set<int> m_usPilgrimSeqList;        //서버에 존재하는 캐릭터 고유 시퀀스 목록

public:
    LoginPlayer(ObjectTag);
    virtual ~LoginPlayer() = default;

    virtual void Reset();

};

