#include "PGPPrivate.h"
#include "ServerConfig.h"
#include <filesystem>
#include <fstream>
#include <json/json.h>

#include "Base64Util.h"
#include "XORUtil.h"

namespace fs = std::filesystem;
//------------------------------------------------------------------
bool ServerConfigData::LoadConfig(const std::wstring& _configFile)
{
    if (true == m_bIsLoaded)
        return false;

    if (true == _configFile.empty())
        return false;

    std::error_code localEC;
    auto localCurrentDir = fs::current_path(localEC);
    if (localEC.value() != 0)
        return false;   //파일 경로 호출 에러

    std::wstring localwsPath(localCurrentDir.c_str() + std::wstring(L"\\") + _configFile);

    auto localFilePath = fs::path(localwsPath);

    if (false == fs::exists(localFilePath, localEC))
        return false;
    if (false == fs::is_regular_file(localFilePath, localEC))
        return false;

    auto localFileSize = fs::file_size(localFilePath, localEC);
    if (localEC.value() != 0)
        return false;

    m_sConfigFileName = _configFile;

    m_bIsLoaded = _LoadConfig(localwsPath.c_str(), localFileSize);
    _LoadCPUCount();


    return m_bIsLoaded;
}

bool ServerConfigData::Reload()
{
    if (true == m_bIsLoaded)
        return false;

    std::error_code localEC;
    auto localCurrentDir = fs::current_path(localEC);
    if (localEC.value() != 0)
        return false;   //파일 경로 호출 에러

    std::wstring localwsPath(localCurrentDir.c_str() + std::wstring(L"\\") + m_sConfigFileName);

    auto localFilePath = fs::path(localwsPath);

    if (false == fs::exists(localFilePath, localEC))
        return false;
    if (false == fs::is_regular_file(localFilePath, localEC))
        return false;

    auto localFileSize = fs::file_size(localFilePath, localEC);
    if (localEC.value() != 0)
        return false;

    return _LoadConfig(localwsPath.c_str(), localFileSize);
}

const ServerListenerInfo& ServerConfigData::GetMainListenerInfo() const
{
    return m_oMainListenInfo;
}

const ServerListenerInfo* ServerConfigData::GetSubListenerInfo(const std::string& _subName) const
{
    if (auto localIT = m_umSubListenInfo.find(_subName); localIT != m_umSubListenInfo.end())
        return &localIT->second;

    return nullptr;
}

const ConnectorInfo* ServerConfigData::GetConnectorInfo(const std::string& _connectName) const
{
    if (auto localIT = m_umConnectorInfo.find(_connectName); localIT != m_umConnectorInfo.end())
        return &localIT->second;

    return nullptr;
}

const DBInfo* ServerConfigData::GetDBConnectionInfo(const std::string& _dbTypeName) const
{
    if (auto localIT = m_umDBConnectionInfo.find(_dbTypeName); localIT != m_umDBConnectionInfo.end())
        return &localIT->second;

    return nullptr;
}

const std::unordered_map<int, DBInfo>& ServerConfigData::GetGDBConnectionInfo() const
{
    return m_umGDBConnectionInfo;
}

const std::unordered_map<int, DBInfo>& ServerConfigData::GetLDBConnectionInfo() const
{
    return m_umLDBConnectionInfo;
}

std::string ServerConfigData::GetLogDir() const
{
    return m_sLogDir;
}

const int& ServerConfigData::GetLogLevel() const
{
    return m_nLogLvl;
}

std::string ServerConfigData::GetDumpDir() const
{
    return m_sDumpDir;
}

const int ServerConfigData::GetObjectPoolSize(const std::string& _strName) const
{
    if (auto localIT = m_umObjectPoolInfo.find(_strName); localIT != m_umObjectPoolInfo.end())
        return localIT->second;

    return 0;
}

std::string ServerConfigData::GetMapInfoPath() const
{
    return m_sMapInfoDir;
}

const bool& ServerConfigData::GetTracePacket() const
{
    return m_bTracePacket;
}

const bool& ServerConfigData::GetTracePacketIgnoreBattle() const
{
    return m_bTracePacketIgnoreBattle;
}

const bool& ServerConfigData::UseMDB() const
{
    return m_bUseMDB;
}

std::string ServerConfigData::GetMDBPath() const
{
    return m_sMDBPath;
}

const bool& ServerConfigData::UseCheckProtocol() const
{
    return m_bProtocolCheck;
}

const int& ServerConfigData::GetCPUCount() const
{
    return m_nProcessCount;
}

bool ServerConfigData::IsLoaded() const
{
    return m_bIsLoaded;
}

std::unordered_map<std::string, ConnectorInfo>& ServerConfigData::GetGameServerList()
{
    return m_umGameServers;
}

std::unordered_map<std::string, ConnectorInfo>& ServerConfigData::GetLoginServerList()
{
    return m_umLoginServers;
}

std::unordered_map<std::string, ConnectorInfo>& ServerConfigData::GetMessengerServerList()
{
    return m_umMessengerServers;
}

std::unordered_map<std::string, ConnectorInfo>& ServerConfigData::GetLogServerList()
{
    return m_umLogServers;
}

bool ServerConfigData::GetMessengerServerInfo([[maybe_unused]] int serverGroupID, ConnectorInfo& _connectorInfo)
{
    if (m_umMessengerServers.empty())
        return false;

    _connectorInfo = m_umMessengerServers.begin()->second;
    return true;
}

const int& ServerConfigData::GetServerID() const
{
    return m_oMainListenInfo.m_nServerID;
}

EServer::Type ServerConfigData::GetServerType() const
{
    return m_oMainListenInfo.m_eServerType;
}

const int& ServerConfigData::GetClientVer() const
{
    return m_oMainListenInfo.m_nClientVer;
}

const ELang& ServerConfigData::GetLanguageType() const
{
    return m_eLanguage;
}

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
     *              "PublicHost": "0.0.0.0",
     *              "PublicPort": 1231
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

            ServerListenerInfo localInsertInfo;
            localInsertInfo.m_nServerGroupID = localSubListenInfo.get("ServerGroupID", 0).asInt();
            localInsertInfo.m_nServerID = localSubListenInfo.get("ServerID", 0).asInt();
            localInsertInfo.m_sBindAddress = localSubListenInfo.get("BindAddress", 0).asString();
            localInsertInfo.m_nBindPort = localSubListenInfo.get("BindPort", 0).asInt();

            localInsertInfo.m_sPublicHost = localSubListenInfo.get("PublicHost", 0).asString();
            localInsertInfo.m_nPublicPort = localSubListenInfo.get("PublicPort", 0).asInt();

            if (localInsertInfo.m_sPublicHost.empty() || localInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'GameServerList' Info" << std::endl;
                continue;
            }

            m_umGameServers.insert(std::pair(localSubName, localInsertInfo));
            m_umServerTypeByServerID.insert_or_assign(localInsertInfo.m_nServerID, EServer::Game);
        }
    }

    //LoginServer List
    m_umLoginServers.clear();
    /*!
     *  JsonExample) Root 아래에 넣어야한다.
     *  {
     *      LoginServerList:{
     *          "1" : {
     *              "ServerID": 10101,
     *              "IP": "0.0.0.0",
     *              "Port": 35202,
     *              "PublicHost": "",
     *              "PublicPort": ""
     *          }
     *      }
     *  }
     */
    if (const auto& localSubListen = localRoot["LoginServerList"]; false == localSubListen.isNull())
    {
        for (const auto& localSubName : localSubListen.getMemberNames())
        {
            const auto& localInfo = localSubListen[localSubName];
            if (true == localInfo.isNull())
                continue;

            ServerListenerInfo localInsertInfo;
            localInsertInfo.m_nServerID = localInfo.get("ServerID", 0).asInt();
            localInsertInfo.m_sBindAddress = localInfo.get("IP", "127.0.0.1").asString();
            localInsertInfo.m_nBindPort = localInfo.get("Port", 0).asInt();

            localInsertInfo.m_sPublicHost = localInfo.get("PublicHost", 0).asString();
            localInsertInfo.m_nPublicPort = localInfo.get("PublicPort", 0).asInt();

            if (localInsertInfo.m_sPublicHost.empty() || localInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'LoginServerList' Info" << std::endl;
                continue;
            }

            m_umLoginServers.insert(std::pair(localSubName, localInsertInfo));

            m_umServerTypeByServerID.insert_or_assign(localInsertInfo.m_nServerID, EServer::Login);
        }

    }

    //MessengerServer List
    m_umMessengerServers.clear();
    if (const auto& localSubListen = localRoot["MessengerServerList"]; false == localSubListen.isNull())
    {
        for (const auto& localSubName : localSubListen.getMemberNames())
        {
            const auto& localInfo = localSubListen[localSubName];
            if (true == localInfo.isNull())
                continue;

            ServerListenerInfo localInsertInfo;
            localInsertInfo.m_nServerGroupID = localInfo.get("ServerGroupID", 0).asInt();
            localInsertInfo.m_nServerID = localInfo.get("ServerID", 0).asInt();
            localInsertInfo.m_sBindAddress = localInfo.get("IP", "127.0.0.1").asString();
            localInsertInfo.m_nBindPort = localInfo.get("Port", 0).asInt();

            localInsertInfo.m_sPublicHost = localInfo.get("PublicHost", 0).asString();
            localInsertInfo.m_nPublicPort = localInfo.get("PublicPort", 0).asInt();

            if (localInsertInfo.m_sPublicHost.empty() || localInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'MessengerServer' Info" << std::endl;
                continue;
            }

            m_umMessengerServers.insert(std::pair(localSubName, localInsertInfo));

            m_umServerTypeByServerID.insert_or_assign(localInsertInfo.m_nServerID, EServer::Messenger);
        }
    }

    //LogServerList Start
    m_umLogServers.clear();
    if (const auto& localSubListen = localRoot["LogServerList"]; false == localSubListen.isNull())
    {
        for (const auto& localSubName : localSubListen.getMemberNames())
        {
            const auto& localInfo = localSubListen[localSubName];
            if (true == localInfo.isNull())
                continue;

            ServerListenerInfo localInsertInfo;
            localInsertInfo.m_nServerID = localInfo.get("ServerID", 0).asInt();
            localInsertInfo.m_sBindAddress = localInfo.get("IP", "127.0.0.1").asString();
            localInsertInfo.m_nBindPort = localInfo.get("Port", 0).asInt();

            if (localInsertInfo.m_sPublicHost.empty() || localInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'LogServerList' Info" << std::endl;
                continue;
            }

            m_umLogServers.insert(std::pair(localSubName, localInsertInfo));

            auto localFindIter = m_umServerTypeByServerID.find(localInsertInfo.m_nServerID);

            if (localFindIter == m_umServerTypeByServerID.end())
            {
                std::cout << "Check LogServer Config! LogServerID (" << localSubName << ") not matched GameServer" << std::endl;
                return false;
            }

            if (localFindIter->second != EServer::Game)
            {
                std::cout << "Check LogServer Config! LogServerID (" << localSubName << ") not matched GameServer" << std::endl;
                return false;
            }


            m_umServerTypeByServerID.insert_or_assign(localInsertInfo.m_nServerID, EServer::Log);
        }
    }
    //LogServerList End

    //Sub Listener
    m_umSubListenInfo.clear();
    if (const auto& localSubListen = localRoot["SubListener"]; false == localSubListen.isNull())
    {
        for (const auto& localSubName : localSubListen.getMemberNames())
        {
            const auto& localSubListenInfo = localSubListen[localSubName];
            if (true == localSubListenInfo.isNull())
                continue;

            ServerListenerInfo localInsertInfo;
            localInsertInfo.m_sBindAddress = localSubListenInfo.get("BindAddress", 0).asString();
            localInsertInfo.m_nBindPort = localSubListenInfo.get("BindPort", 0).asInt();

            m_umSubListenInfo.insert(std::pair(localSubName, localInsertInfo));
        }
    }
    //end Sub Listener

    //Connector
    m_umConnectorInfo.clear();
    if (const auto& localConnector = localRoot["Connect"]; false == localConnector.isNull())
    {
        for (const auto& localSubName : localConnector.getMemberNames())
        {
            const auto& localConnectorInfo = localConnector[localSubName];
            if (true == localConnectorInfo.isNull())
                continue;

            ConnectorInfo localInsertInfo;
            localInsertInfo.m_sBindAddress = localConnectorInfo.get("Host", 0).asString();
            localInsertInfo.m_nBindPort = localConnectorInfo.get("Port", 0).asInt();

        }
    }

    //DB Integrated
    m_umDBConnectionInfo.clear();
    if (const auto& localDB = localRoot["DB"]; false == localDB.isNull())
    {
        for (const auto& localName : localDB.getMemberNames())
        {
            const auto& localDBInfo = localDB[localName];
            if (true == localDBInfo.isNull())
                continue;

            DBInfo localInertInfo;
            localInertInfo.m_sDBHost = localDBInfo.get("Host", "127.0.0.1").asString();
            localInertInfo.m_nDBPort = localDBInfo.get("Port", 1433).asInt();
            localInertInfo.m_sDBName = localDBInfo.get("DATABASE", "").asString();

            localInertInfo.m_sUserID = localDBInfo.get("UID", "server").asString();
            localInertInfo.m_nThreadCount = localDBInfo.get("Thread", 0).asInt();

            std::string localTmpPwd = localDBInfo.get("Password", "").asString();

            char localTmp[1024] = { 0, };
            char localTmpForXOR[1024] = { 0, };

            size_t localDecodeSize = localBase64.Decode(localTmpPwd, localTmp, 1024);

            if (true == localXORUtil.Encrypt(localTmp, localDecodeSize, localTmpForXOR, 1024))
            {
                localInertInfo.m_sPassword = std::string(localTmpForXOR);
            }

            m_umDBConnectionInfo.insert(std::pair(localName, localInertInfo));
        }
    }
    // end DataBase

    // GDB for MessengerServer
    m_umGDBConnectionInfo.clear();
    if (const auto& localDB = localRoot["GDB"]; false == localDB.isNull())
    {
        for (const auto& localName : localDB.getMemberNames())
        {
            const auto& localDBInfo = localDB[localName];
            if (true == localDBInfo.isNull())
                continue;
            DBInfo localInertInfo;
            localInertInfo.m_sDBHost = localDBInfo.get("Host", "127.0.0.1").asString();
            localInertInfo.m_nDBPort = localDBInfo.get("Port", 1433).asInt();
            localInertInfo.m_sDBName = localDBInfo.get("DATABASE", "").asString();

            localInertInfo.m_sUserID = localDBInfo.get("UID", "server").asString();
            localInertInfo.m_nThreadCount = localDBInfo.get("Thread", 0).asInt();

            std::string localTmpPwd = localDBInfo.get("Password", "").asString();

            char localTmp[1024] = { 0, };
            char localTmpForXOR[1024] = { 0, };

            size_t localDecodeSize = localBase64.Decode(localTmpPwd, localTmp, 1024);

            if (true == localXORUtil.Encrypt(localTmp, localDecodeSize, localTmpForXOR, 1024))
            {
                localInertInfo.m_sPassword = std::string(localTmpForXOR);
            }

            m_umGDBConnectionInfo.insert(std::pair(stoi(localName), localInertInfo));

            int localServerID = std::atoi(localName.c_str());
            auto localFindIter = m_umServerTypeByServerID.find(localServerID);

            if (localFindIter == m_umServerTypeByServerID.end())
            {
                std::cout << "Check GDB Config! ServerID (" << localName << ") not matched GameServer" << std::endl;
                return false;
            }

            if (localFindIter->second != EServer::Game)
            {
                std::cout << "Check GDB Config! ServerID (" << localName << ") not matched GameServer" << std::endl;
                return false;
            }
        }
    }
    // GDB for MessengerServer End

    //LDB
    m_umLDBConnectionInfo.clear();
    if (const auto& localDB = localRoot["LDB"]; false == localDB.isNull())
    {
        for (const auto& localName : localDB.getMemberNames())
        {
            const auto& localDBInfo = localDB[localName];
            if (true == localDBInfo.isNull())
                continue;

            DBInfo localInertInfo;
            localInertInfo.m_sDBHost = localDBInfo.get("Host", "127.0.0.1").asString();
            localInertInfo.m_nDBPort = localDBInfo.get("Port", 1433).asInt();
            localInertInfo.m_sDBName = localDBInfo.get("DATABASE", "").asString();

            localInertInfo.m_sUserID = localDBInfo.get("UID", "server").asString();
            localInertInfo.m_nThreadCount = localDBInfo.get("Thread", 0).asInt();

            std::string localTmpPwd = localDBInfo.get("Password", "").asString();

            char localTmp[1024] = { 0, };
            char localTmpForXOR[1024] = { 0, };

            size_t localDecodeSize = localBase64.Decode(localTmpPwd, localTmp, 1024);

            if (true == localXORUtil.Encrypt(localTmp, localDecodeSize, localTmpForXOR, 1024))
            {
                localInertInfo.m_sPassword = std::string(localTmpForXOR);
            }

            m_umLDBConnectionInfo.insert(std::pair(stoi(localName), localInertInfo));
        }
    }
    // end LDB

    //ObjectPool
    m_umObjectPoolInfo.clear();
    if (const auto& localPool = localRoot["ObjectPools"]; false == localPool.isNull())
    {
        for (const auto& localName : localPool.getMemberNames())
        {
            const auto& localPoolInfo = localPool[localName];
            if (true == localPoolInfo.isNull())
                continue;

            int localVal = localPool.get(localName, 0).asInt();

            m_umObjectPoolInfo.insert(std::pair(localName, localVal));
        }
    }


    //Protocol Checker
    if (const auto& localProtocolCheck = localRoot["ProtocolChecker"]; false == localProtocolCheck.isNull())
    {
        m_bProtocolCheck = localProtocolCheck.get("Enable", false).asBool();
    }

    //TracePacket
    if (const auto& localTracePacket = localRoot["TracePacket"]; false == localTracePacket.isNull())
    {
        m_bTracePacket = localTracePacket.get("Enable", false).asBool();
        m_bTracePacketIgnoreBattle = localTracePacket.get("IgnoreBattle", false).asBool();
    }

    //Use MDB
    if (const auto& localMDB = localRoot["MDB"]; false == localMDB.isNull())
    {
        m_bUseMDB = localMDB.get("Enable", false).asBool();
        m_sMDBPath = localMDB.get("Path", "").asString();
    }

    //log
    if (const auto& localLog = localRoot["Log"]; false == localLog.isNull())
    {
        m_sLogDir = localLog.get("Dir", "../Log/").asString();
        m_nLogLvl = localLog.get("Level", 0).asInt();
    }


    //Map
    m_sMapInfoDir = localRoot.get("MapDir", "").asString();

    //DumpDir
    m_sDumpDir = localRoot.get("DumpDir", "").asString();
    return true;
}

void ServerConfigData::_Clear()
{
    m_umSubListenInfo.clear();
    m_umConnectorInfo.clear();
    m_umDBConnectionInfo.clear();
    m_umGDBConnectionInfo.clear();
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

//------------------------------------------------------------------

bool ServerConfig::LoadConfig(const std::wstring& _confFile)
{
    if (true == m_bIsFirstLoaded)
        return false;

    m_nSwitch.store(0);
    for (int i = 0; i < CONFIG_SWITCH_SIZE; i++)
    {
        if (false == m_oServerConfigData[i].LoadConfig(_confFile))
            return false;
    }

    m_sConfigFileName = _confFile;
    m_bIsFirstLoaded = true;
    return true;
}

bool ServerConfig::ReloadConfig()
{
    int localIdx = (m_nSwitch.load() + 1) % CONFIG_SWITCH_SIZE;

    ServerConfigData localLoadData;
    if (false == localLoadData.LoadConfig(m_sConfigFileName))
        return false;

    std::swap(m_oServerConfigData[localIdx], localLoadData);

    m_nSwitch.store(localIdx);
    return true;
}

ServerConfigData& ServerConfig::GetConfig()
{
    int localIdx = m_nSwitch.load() % CONFIG_SWITCH_SIZE;
    return m_oServerConfigData[localIdx];
}

int ServerConfig::GetServerID()
{
    return ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_nServerID;
}

bool ServerConfig::IsMessengerServer()
{
    return ServerConfig::GetInst().GetConfig().GetServerType() == EServer::Type::Messenger ? true : false;
}
