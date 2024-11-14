#include "pch.h"
#include "PGameServerApp.h"

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
    return false;
}

bool PGameServerApp::RunLoop()
{
    return false;
}

void PGameServerApp::SendServerLog(const std::wstring& _remark)
{
}

void PGameServerApp::_InitLog()
{
}
