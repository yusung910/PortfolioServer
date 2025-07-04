#include "pch.h"
#include "LoginPlayer.h"

LoginPlayer::LoginPlayer(ObjectTag)
{
}

void LoginPlayer::Reset()
{
    m_eClientType = EClient::Type::None;
    m_eBuildType = EBuild::Type::None;

    m_nAppVersion = 0;
    m_nBuildStamp = 0;

    m_sAuthKey.clear();
    m_nAccountSeq = 0;
    m_nSelectedServerID = 0;

    m_bIsAllowed = false;

    m_eState = ELoginState::None;
    m_nHostID = 0;

    m_sReturnToken.clear();
    m_sPlatformID.clear();
    m_sPlatformID.clear();

    m_nOTP = 0;

    m_usPilgrimSeqList.clear();
}
