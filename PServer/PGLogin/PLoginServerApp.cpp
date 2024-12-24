#include "pch.h"
#include "PLoginServerApp.h"
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

    return true;
}

bool PLoginServerApp::RunLoop()
{
    return false;
}

void PLoginServerApp::_InitLog()
{
}

bool PLoginServerApp::_InitUserDB()
{
    return false;
}
