#include "pch.h"
#include "PLoginServerApp.h"
#include "GameServerCheckService.h"
#include "LoginDBLoadBalancer.h"
#include "LoginDBService.h"
#include "LoginService.h"
#include "LoginServerNES.h"
#include "ServerConst.h"
#include <ServerConfig.h>
#include <ServerApp.h>


#include <NetworkManager.h>

PLoginServerApp::PLoginServerApp()
{
    //
    LoggingManager::GetInst().Init();
    NetworkManager::GetInst().Init();
}

PLoginServerApp::~PLoginServerApp()
{
    LoggingManager::GetInst().Stop();
}

bool PLoginServerApp::Initialize()
{
    //서버 설정(ServerConfig.json)을 불러온다
    ServerConfig::GetInst().LoadConfig();
    GameServerCheckService::GetInst().LoadGameServers();

    _InitLog();
    
    //회사 PC에서는 DB접근이 안되도록해야한다.
    if (false == _InitAccountDB())
        return false;

    ServerConst::GetInst().SetDefault();
    auto lService = LoginDBLoadBalancer::GetInst().GetDirectService<LoginDBService>();
    ServerConst::GetInst().LoadServerConst(lService);
    return true;
}

bool PLoginServerApp::RunLoop()
{
    //Packet Pool
    Packet::ReservePool(ServerConfig::GetInst().GetConfig().GetObjectPoolSize("Packet"));

    if (false == LoginService::GetInst().Start())
        return false;

    if (false == GameServerCheckService::GetInst().Start())
        return false;

    //MainLoop
    auto& lMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();
    VIEW_SYSTEM("Server Configuration Load Complated Profile Service:%s ID:%d", lMainInfo.m_sServiceName.c_str(), lMainInfo.m_nServerID);

    auto lApp = std::make_shared<ServerApp>();
    auto lEventSync = std::make_shared<LoginServerNES>();

    //액션 없을 때 자동으로 연결 끊는 시간 설정 삭제
    //lEventSync->SetTimeoutMS(INT_MAX);

    lApp->SetListenerInfo(lMainInfo);
    lApp->BindEventSync(lEventSync);
    //SendServerLog(L"Live!");
    lApp->Run();

    LoginService::GetInst().Exit();
    GameServerCheckService::GetInst().Exit();

    return true;
}

void PLoginServerApp::_InitLog()
{
    auto& lMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();

    LoggingManager::GetInst().SetServerName(lMainInfo.m_sServiceName);
    LoggingManager::GetInst().SetLogDirConfig(ServerConfig::GetInst().GetConfig().GetLogDir());
    LoggingManager::GetInst().SetServerNum(lMainInfo.m_nServerID);
    LoggingManager::GetInst().Start();
}

bool PLoginServerApp::_InitAccountDB()
{
    auto lInfo = ServerConfig::GetInst().GetConfig().GetDBConnectionInfo("ADB");
    if (nullptr == lInfo)
    {
        VIEW_WRITE_ERROR("Account DB Connection Info is null");
        return false;
    }

    LoginDBLoadBalancer::GetInst().SetDBConfig<LoginDBService>(lInfo->m_sUserID, lInfo->m_sPassword, lInfo->m_sDBName, lInfo->m_sDBHost, std::to_string(lInfo->m_nDBPort), lInfo->m_nThreadCount);
    
    return true;
}
