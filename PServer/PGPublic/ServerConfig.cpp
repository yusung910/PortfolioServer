#include "PGPPrivate.h"
#include "ServerConfig.h"
#include <filesystem>
#include <fstream>
#include <json/json.h>

#include "Base64Util.h"
#include "XORUtil.h"

namespace fs = std::filesystem;

bool ServerConfigData::_LoadConfig(const std::wstring& _confFile, size_t _fileSize)
{
    if (true == _confFile.empty())
        return false;

    if (_fileSize == 0 || _fileSize == (uintmax_t)~0)
        return false;

    std::ifstream localIS(_confFile);
    if (false == localIS.is_open())
        return false;

    std::string localStrBuffer;
    localStrBuffer.reserve(_fileSize);

    std::string localStrLine;
    while (getline(localIS, localStrLine))
        localStrBuffer.append(localStrLine);

    localIS.close();

    Json::CharReaderBuilder localJsonBuilder;
    auto localReader = localJsonBuilder.newCharReader();
    JSONCPP_STRING localErr;
    Json::Value localRoot;

    if (false == localReader->parse(localStrBuffer.c_str(), localStrBuffer.c_str() + _fileSize, &localRoot, &localErr))
        return false;


    //암,복호화
    XORUtil localXORUtil;
    Base64Util localBase64;

    //MainListener
    {
        /*!
         *  JSON example)
         *  {
         *      "MainListener" : {
         *          "Name"                  : "",
         *          "ServerID"              : 10000,
         *          "BindAddress"           : "0.0.0.0",
         *          "BindPort"              : 35201,
         *          "TimeoutMS"             : 3600000,
         *          "MaxConnection"         : 3000,
         *          "ControlConnection"     : 1500,
         *          "SendContextLimitCount" : 500,
         *          "ServiceType"           : "Dev"
         *      }
         *  }
         */

        const auto& localMainListenerInfo = localRoot["MainListener"];

        //MainListener가 없을 경우 종료
        if (true == localMainListenerInfo.isNull())
            return false;

        m_oMainListenInfo.m_sServiceName = localMainListenerInfo.get("Name", "Server").asString();
        m_oMainListenInfo.m_nServerID = localMainListenerInfo.get("ServerID", 0).asInt();
        m_oMainListenInfo.m_sBindAddress = localMainListenerInfo.get("BindAddress", "127.0.0.1").asString();
        m_oMainListenInfo.m_nBindPort = localMainListenerInfo.get("BindPort", 0).asInt();
        m_oMainListenInfo.m_nTimeoutMS = localMainListenerInfo.get("TimeoutMS", DEFAULT_TIMEOUT_MS).asInt();
        m_oMainListenInfo.m_nMaxConnection = localMainListenerInfo.get("MaxConnection", DEFAULT_CONNECTION_COUNT).asInt();
        m_oMainListenInfo.m_nControlConnection = localMainListenerInfo.get("ControlConnection", DEFAULT_CONNECTION_CONTROL_COUNT).asInt();
        m_oMainListenInfo.m_nSendContextLimitCount = localMainListenerInfo.get("SendContextLimitCount", DEFAULT_SEND_CONTEXT_LIMIT_COUNT).asInt();
        m_oMainListenInfo.m_sServiceType = localMainListenerInfo.get("ServiceType", "Live").asString();

        if (m_oMainListenInfo.m_sServiceType == "Live")
        {
            m_oMainListenInfo.m_bIsLive = true;
        }
        else
        {
            m_oMainListenInfo.m_bIsLive = false;
        }

        m_oMainListenInfo.m_nClientVer = localMainListenerInfo.get("ClientVer", 0).asInt();

        if (m_oMainListenInfo.m_sServiceName == "Login")
            m_oMainListenInfo.m_eServerType = EServer::Login;
        else if (m_oMainListenInfo.m_sServiceName == "Game")
            m_oMainListenInfo.m_eServerType = EServer::Game;
        else if (m_oMainListenInfo.m_sServiceName == "Log")
            m_oMainListenInfo.m_eServerType = EServer::Log;
        else if (m_oMainListenInfo.m_sServiceName == "Platform")
            m_oMainListenInfo.m_eServerType = EServer::Platform;
        else if (m_oMainListenInfo.m_sServiceName == "Messenger")
            m_oMainListenInfo.m_eServerType = EServer::Messenger;

        m_oMainListenInfo.m_bIsMaintenanceMode = localMainListenerInfo.get("MaintenanceMode", false).asBool();
    }

    //GameServer List
    m_umGameServers.clear();
    /*!
     *  JsonExample) Root 아래에 넣어야한다.
     *  {
     *      GameServerList:{
     *          "10101" : {
     *              "ServerGroupID": 1,
     *              "ServerID": 10101,
     *              "BindAddress": "0.0.0.0",
     *              "BindPort": 35202,
     *              "PublicHost": "",
     *              "PublicPort": ""
     *          }
     *      }
     *  }
     */
    if (const auto& localSubListen = localRoot["GameServerList"]; false == localSubListen.isNull())
    {
        for (const auto& localSubName : localSubListen.getMemberNames())
        {
            const auto& localSubListenInfo = localSubListen[localSubName];

            if (true == localSubListenInfo.isNull())
                continue;

            ServerListenerInfo insertSubListenerInfo;
            insertSubListenerInfo.m_nServerGroupID = localSubListenInfo.get("ServerGroupID", 0).asInt();
            insertSubListenerInfo.m_nServerID = localSubListenInfo.get("ServerID", 0).asInt();
            insertSubListenerInfo.m_sBindAddress = localSubListenInfo.get("BindAddress", 0).asString();
            insertSubListenerInfo.m_nBindPort = localSubListenInfo.get("BindPort", 0).asInt();

            insertSubListenerInfo.m_sPublicHost = localSubListenInfo.get("PublicHost", 0).asString();
            insertSubListenerInfo.m_nPublicPort = localSubListenInfo.get("PublicPort", 0).asInt();

            if (insertSubListenerInfo.m_sPublicHost.empty() || insertSubListenerInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'GameServerList' Info" << std::endl;
                continue;
            }

            m_umGameServers.insert(std::pair(localSubName, insertSubListenerInfo));
            m_umServerTypeByServerID.insert_or_assign(insertSubListenerInfo.m_nServerID, EServer::Game);
        }
    }

    return true;
}

void ServerConfigData::_Clear()
{
    m_umSubListenInfo.clear();
    m_umConnectorInfo.clear();
    m_umDBConnectionInfo.clear();
    m_umADBConnectionInfo.clear();
    m_umGDBConnectionInfo.clear();
    m_umEDBConnectionInfo.clear();
    m_umLDBConnectionInfo.clear();

    m_umObjectPoolInfo.clear();

    m_bIsLoaded = false;
}

void ServerConfigData::_LoadCPUCount()
{
    SYSTEM_INFO localInfo;
    GetSystemInfo(&localInfo);

    m_nProcessCount = (int)localInfo.dwNumberOfProcessors;

    if (m_nProcessCount <= 0)
        m_nProcessCount = 0;
}
