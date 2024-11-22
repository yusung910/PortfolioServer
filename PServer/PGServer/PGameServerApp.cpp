#include "pch.h"
#include "PGameServerApp.h"
#include <ServerConfig.h>
#include <ServerApp.h>
#include "GSNetworkEventSync.h"

#include "GameService.h"

#include <NetworkManager.h>

PGameServerApp::PGameServerApp()
{
    //
    LoggingManager::GetInst().Init();
    NetworkManager::GetInst().Init();
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

    //USerDB ����

    //SendServerLog(L"SendServerLog Initialize...");


    return true;
}

bool PGameServerApp::RunLoop()
{
    //Packet Pool
    Packet::ReservePool(ServerConfig::GetInst().GetConfig().GetObjectPoolSize("Packet"));

    //Service ����!
    if (false == GameService::GetInst().Start())
    {
        return false;
    }

    //MainLoop
    auto& localMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();
    VIEW_SYSTEM("Server Configuration Load Complated Profile Service:%s ID:%d", localMainInfo.m_sServiceName.c_str(), localMainInfo.m_nServerID);

    auto localApp = std::make_shared<ServerApp>();
    auto localEventSync = std::make_shared<GSNetworkEventSync>();

    //�׼� ���� �� �ڵ����� ���� ���� �ð� ���� ����
    //localEventSync->SetTimeoutMS(INT_MAX);

    localApp->SetListenerInfo(localMainInfo);
    localApp->BindEventSync(localEventSync);
    //SendServerLog(L"Live!");
    localApp->Run();


    GameService::GetInst().GetInst().Exit();

    return true;
}

//void PGameServerApp::SendServerLog(const std::wstring& _remark)
//{
//}

void PGameServerApp::_InitLog()
{
    auto& localMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();

    LoggingManager::GetInst().SetServerName(localMainInfo.m_sServiceName);
    LoggingManager::GetInst().SetLogDirConfig(ServerConfig::GetInst().GetConfig().GetLogDir());
    LoggingManager::GetInst().SetServerNum(localMainInfo.m_nServerID);
    LoggingManager::GetInst().Start();
}
