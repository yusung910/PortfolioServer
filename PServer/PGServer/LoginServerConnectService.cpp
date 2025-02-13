#include "pch.h"
#include "LoginServerConnectService.h"

#include <ServerConfig.h>

LoginServerConnectService::LoginServerConnectService()
{
}

LoginServerConnectService::~LoginServerConnectService()
{

}

bool LoginServerConnectService::Start()
{
    auto lMainListenerInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();

    m_nMaxConnection = lMainListenerInfo.m_nMaxConnection;

    m_nWaitingSeq = 0;
    m_nAllowedSeq = 0;

    m_nServerID = lMainListenerInfo.m_nServerID;
    m_oEventSync.SetTimeoutMS(INT_MAX);

    return CreateThread();
}

void LoginServerConnectService::OnListen()
{
    m_bIsStarted = true;
    m_bIsTryListening = false;

    VIEW_INFO("LoginServerConnect Listen Socket Create Success. [Port :%d]", m_nBindPort);
}

void LoginServerConnectService::OnListenFailed()
{
    m_bIsStarted = false;
    m_bIsTryListening = false;
    VIEW_INFO("LoginServerConnect Listen Socket Create Fail. [Port :%d]", m_nBindPort);
}

void LoginServerConnectService::SendToLoginServers(const int& _msgID, void* _msg, const size_t& _msgSize)
{
    AutoLock(m_xHostLock);

    if (true == m_umConnectedLoginServeHostIDList.empty())
        return;

    Packet::SharedPtr lPackt = Packet::New();
    if (false == lPackt->SetPacketData(_msgID, _msg, _msgSize))
    {
        VIEW_ERROR("LoginServerConnect::SendToLoginServers() Failed!! - Didn't made a Packet Data");
    }
    
}
