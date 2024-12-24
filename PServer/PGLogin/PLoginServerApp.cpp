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
    //���� ����(ServerConfig.json)�� �ҷ��´�
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
