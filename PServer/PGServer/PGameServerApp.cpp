#include "pch.h"
#include "PGameServerApp.h"
#include <ServerConfig.h>
#include <ServerApp.h>
#include "GSNetworkEventSync.h"

#include "GameService.h"
#include "GameDBService.h"
#include "GameDBLoadBalancer.h"

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

    //GameDB ����
    _InitGameDB();
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

bool PGameServerApp::_InitGameDB()
{
    auto localDBInfo = ServerConfig::GetInst().GetConfig().GetDBConnectionInfo("GDB");

    if (nullptr == localDBInfo)
    {
        VIEW_WRITE_ERROR("Game DB Connection Info is Null");
        return false;
    }

    GameDBLoadBalancer::GetInst().SetDBConfig<GameDBService>(localDBInfo->m_sUserID, localDBInfo->m_sPassword, localDBInfo->m_sDBName, localDBInfo->m_sDBHost, std::to_string(localDBInfo->m_nDBPort), localDBInfo->m_nThreadCount);


    //auto localService = GameDBLoadBalancer::GetInst().GetDirectService<GameDBService>();
    //if (localService != nullptr)
    //{
    //    //auto localpSession = localService->GetSession();
    //    {
    //        //CSession& localSession = *localpSession;
    //        //auto locaCurTick = Clock::GetTick64();

    //        //BEGIN_SESSION
    //        //END_SESSION

    //    }
    //}
    return true;
}
