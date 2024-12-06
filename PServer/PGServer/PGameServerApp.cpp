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
    //서버 설정(ServerConfig.json)을 불러온다
    ServerConfig::GetInst().LoadConfig();

    //로그 기록 시작
    _InitLog();

    //GameDB 세팅
    _InitGameDB();
    //SendServerLog(L"SendServerLog Initialize...");


    return true;
}

bool PGameServerApp::RunLoop()
{
    //Packet Pool
    Packet::ReservePool(ServerConfig::GetInst().GetConfig().GetObjectPoolSize("Packet"));

    //Service 시작!
    if (false == GameService::GetInst().Start())
    {
        return false;
    }

    //MainLoop
    auto& localMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();
    VIEW_SYSTEM("Server Configuration Load Complated Profile Service:%s ID:%d", localMainInfo.m_sServiceName.c_str(), localMainInfo.m_nServerID);

    auto localApp = std::make_shared<ServerApp>();
    auto localEventSync = std::make_shared<GSNetworkEventSync>();

    //액션 없을 때 자동으로 연결 끊는 시간 설정 삭제
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
