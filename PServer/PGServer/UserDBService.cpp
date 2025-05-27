#include "pch.h"
#include "UserDBService.h"
#include "NetworkCenter.h"

#include <ServerConfig.h>

//log서비스도 추가하자


UserDBService::UserDBService()
{
    m_nCurrentServerID = ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_nServerID;
}

UserDBService::~UserDBService()
{
}
