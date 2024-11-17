#include "pch.h"
#include "PGameServerApp.h"
#include <ServerConfig.h>
#include <ServerApp.h>


#include <NetworkManager.h>



PGameServerApp::PGameServerApp()
{
    //
    LoggingManager::GetInst().Initialize();
    NetworkManager::GetInst().Initialize();
}

PGameServerApp::~PGameServerApp()
{
    LoggingManager::GetInst().Stop();
}

bool PGameServerApp::Initialize()
{
    //���� ����(ServerConfig.json)�� �ҷ��´�
    ServerConfig::GetInst().LoadConfig();
    //�α� ��� ����
    _InitLog();


    return true;
}

bool PGameServerApp::RunLoop()
{
    //Packet Pool
    Packet::ReservePool(ServerConfig::GetInst().GetConfig().GetObjectPoolSize("Packet"));

    return true;
}

void PGameServerApp::SendServerLog(const std::wstring& _remark)
{
}

void PGameServerApp::_InitLog()
{
    auto localMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();

    LoggingManager::GetInst().SetServerName(localMainInfo.m_sServiceName);
    LoggingManager::GetInst().SetLogDirConfig(ServerConfig::GetInst().GetConfig().GetLogDir());
    LoggingManager::GetInst().SetServerNum(localMainInfo.m_nServerID);
    LoggingManager::GetInst().Start();
}
