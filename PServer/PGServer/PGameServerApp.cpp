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
    //_InitGameDB();
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
    auto& lMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();
    VIEW_SYSTEM("Server Configuration Load Complated Profile Service:%s ID:%d", lMainInfo.m_sServiceName.c_str(), lMainInfo.m_nServerID);

    auto lApp = std::make_shared<ServerApp>();
    auto lEventSync = std::make_shared<GSNetworkEventSync>();

    //액션 없을 때 자동으로 연결 끊는 시간 설정 삭제
    //lEventSync->SetTimeoutMS(INT_MAX);

    lApp->SetListenerInfo(lMainInfo);
    lApp->BindEventSync(lEventSync);
    //SendServerLog(L"Live!");
    lApp->Run();


    GameService::GetInst().GetInst().Exit();

    return true;
}

//void PGameServerApp::SendServerLog(const std::wstring& _remark)
//{
//}

void PGameServerApp::_InitLog()
{
    auto& lMainInfo = ServerConfig::GetInst().GetConfig().GetMainListenerInfo();

    LoggingManager::GetInst().SetServerName(lMainInfo.m_sServiceName);
    LoggingManager::GetInst().SetLogDirConfig(ServerConfig::GetInst().GetConfig().GetLogDir());
    LoggingManager::GetInst().SetServerNum(lMainInfo.m_nServerID);
    LoggingManager::GetInst().Start();
}

bool PGameServerApp::_InitGameDB()
{
    auto lDBInfo = ServerConfig::GetInst().GetConfig().GetDBConnectionInfo("GDB");

    if (nullptr == lDBInfo)
    {
        VIEW_WRITE_ERROR("Game DB Connection Info is Null");
        return false;
    }

    GameDBLoadBalancer::GetInst().SetDBConfig<GameDBService>(lDBInfo->m_sUserID, lDBInfo->m_sPassword, lDBInfo->m_sDBName, lDBInfo->m_sDBHost, std::to_string(lDBInfo->m_nDBPort), lDBInfo->m_nThreadCount);


    auto lService = GameDBLoadBalancer::GetInst().GetDirectService<GameDBService>();
    if (lService != nullptr)
    {
        // 서버가 실행 된 시간을 DB에 저장한다.
        // 프테 서버에 있는 내용인데 굳이 필요한지는 추후에 확인하여 작업.
        //auto lpSession = lService->GetSession();
        {
            //Poco::Data::Session& lSess = *lpSession;

            //auto locaCurTick = Clock::GetTick64();

            //BEGIN_SESSION
                //lSess << 
            //END_SESSION
        }
    }
    return true;
}
