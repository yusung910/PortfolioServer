#include "pch.h"
#include "LoginServerConnectService.h"
#include "NetworkCenter.h"

#include <NetworkManager.h>
#include <ServerConfig.h>

LoginServerConnectService::LoginServerConnectService()
{
    RegisterHandler(&LoginServerConnectService::OnConnect);
    RegisterHandler(&LoginServerConnectService::OnDisconnect);

    RegisterTimer(TRY_LISTEN_DELAY_MS, std::bind(&LoginServerConnectService::_TryListen, this));
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

bool LoginServerConnectService::OnConnect(int _hostID, [[maybe_unused]] const HostConnect& _msg)
{
    AutoLock(m_xHostLock);
    m_umConnectedLoginServeHostIDList.insert(_hostID);

    //로그인 서버에 전송이 별도로 필요할 경우 패킷 데이터 생성해서 보낸다
    SendToGame(_hostID, EPacketProtocol::LS_Connect, new InnerDataInterface());

    return true;
}

bool LoginServerConnectService::OnDisconnect(int _hostID, [[maybe_unused]] const HostClose& _msg)
{
    AutoLock(m_xHostLock);
    m_umConnectedLoginServeHostIDList.erase(_hostID);
    return false;
}

void LoginServerConnectService::UpdateServerStatus()
{
    //int lAllowLogin = 0;

    //EServerStatus::Type lCurServerStatus = EServerStatus::Good;
}

void LoginServerConnectService::_TryListen()
{
    if (true == m_bIsStarted)
        return;

    if (true == m_bIsTryListening)
        return;

    if (true == NetworkManager::GetInst().IsInitialized())
    {
        auto lLoginServerHost = ServerConfig::GetInst().GetConfig().GetSubListenerInfo("Login");

        if (nullptr != lLoginServerHost)
        {
            m_bIsTryListening = NetworkManager::GetInst().Listen(&m_oEventSync, lLoginServerHost->m_sBindAddress, lLoginServerHost->m_nBindPort);

            m_nBindPort = lLoginServerHost->m_nBindPort;
        }

        if (false == m_bIsTryListening)
            VIEW_WRITE_ERROR("LoginServer Listen Socket Create Fail!");
    }
}

void LoginServerConnectService::_SendToLoginServer(const EPacketProtocol& _msgID, flatbuffers::FlatBufferBuilder& _fbb)
{
    AutoLock(m_xHostLock);

    if (true == m_umConnectedLoginServeHostIDList.empty())
        return;

    Packet::SharedPtr lPackt = Packet::New();
    if (false == lPackt->SetPacketData(_msgID, _fbb.GetBufferPointer(), _fbb.GetSize()))
    {
        VIEW_ERROR("LoginServerConnect::_SendToLoginServer() Failed!! - Didn't made a Packet Data");
        return;
    }

    for (auto& lHostID : m_umConnectedLoginServeHostIDList)
    {
        lPackt->m_nHostID = lHostID;
        NetworkManager::GetInst().Send(lHostID, lPackt);
    }
}

void LoginServerConnectService::_SendToLogin(const int& _hostID, const EPacketProtocol& _msgID, flatbuffers::FlatBufferBuilder& _fbb)
{
    Packet::SharedPtr lPackt = Packet::New();
    lPackt->m_nHostID = _hostID;
    if (true == lPackt->SetPacketData(_msgID, _fbb.GetBufferPointer(), _fbb.GetSize()))
        NetworkManager::GetInst().Send(_hostID, lPackt);
}

bool LoginServerConnectService::_SendErrorMessage(const int& _hostID, const EErrorMsg& _err, const EPacketProtocol& _msgID)
{
    flatbuffers::FlatBufferBuilder lFbb;

    auto lPacket = CreateSCIntegrationErrorNotification(lFbb, (int)_msgID, (int)_err);

    Packet::SharedPtr lPackt = Packet::New();
    lPackt->m_nHostID = _hostID;

    if (true == lPackt->SetPacketData((int)EPacketProtocol::SC_IntegrationErrorNotification, lFbb.GetBufferPointer(), lFbb.GetSize()))
        NetworkManager::GetInst().Send(_hostID, lPackt);

    return false;
}
