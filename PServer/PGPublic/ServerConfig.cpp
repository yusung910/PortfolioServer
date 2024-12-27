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

    std::error_code lEC;
    auto lCurrentDir = fs::current_path(lEC);
    if (lEC.value() != 0)
        return false;   //파일 경로 호출 에러

    std::wstring lwsPath(lCurrentDir.c_str() + std::wstring(L"\\") + _configFile);

    auto lFilePath = fs::path(lwsPath);

    if (false == fs::exists(lFilePath, lEC))
        return false;
    if (false == fs::is_regular_file(lFilePath, lEC))
        return false;

    auto lFileSize = fs::file_size(lFilePath, lEC);
    if (lEC.value() != 0)
        return false;

    m_sConfigFileName = _configFile;

    m_bIsLoaded = _LoadConfig(lwsPath.c_str(), lFileSize);
    _LoadCPUCount();


    return m_bIsLoaded;
}

bool ServerConfigData::Reload()
{
    if (true == m_bIsLoaded)
        return false;

    std::error_code lEC;
    auto lCurrentDir = fs::current_path(lEC);
    if (lEC.value() != 0)
        return false;   //파일 경로 호출 에러

    std::wstring lwsPath(lCurrentDir.c_str() + std::wstring(L"\\") + m_sConfigFileName);

    auto lFilePath = fs::path(lwsPath);

    if (false == fs::exists(lFilePath, lEC))
        return false;
    if (false == fs::is_regular_file(lFilePath, lEC))
        return false;

    auto lFileSize = fs::file_size(lFilePath, lEC);
    if (lEC.value() != 0)
        return false;

    return _LoadConfig(lwsPath.c_str(), lFileSize);
}

const ServerListenerInfo& ServerConfigData::GetMainListenerInfo() const
{
    return m_oMainListenInfo;
}

const ServerListenerInfo* ServerConfigData::GetSubListenerInfo(const std::string& _subName) const
{
    if (auto lIT = m_umSubListenInfo.find(_subName); lIT != m_umSubListenInfo.end())
        return &lIT->second;

    return nullptr;
}

const ConnectorInfo* ServerConfigData::GetConnectorInfo(const std::string& _connectName) const
{
    if (auto lIT = m_umConnectorInfo.find(_connectName); lIT != m_umConnectorInfo.end())
        return &lIT->second;

    return nullptr;
}

const DBInfo* ServerConfigData::GetDBConnectionInfo(const std::string& _dbTypeName) const
{
    if (auto lIT = m_umDBConnectionInfo.find(_dbTypeName); lIT != m_umDBConnectionInfo.end())
        return &lIT->second;

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
    if (auto lIT = m_umObjectPoolInfo.find(_strName); lIT != m_umObjectPoolInfo.end())
        return lIT->second;

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

    std::ifstream lIS(_confFile);
    if (false == lIS.is_open())
        return false;

    std::string lStrBuffer;
    lStrBuffer.reserve(_fileSize);

    std::string lStrLine;
    while (getline(lIS, lStrLine))
        lStrBuffer.append(lStrLine);

    lIS.close();

    Json::CharReaderBuilder lJsonBuilder;
    auto lReader = lJsonBuilder.newCharReader();
    JSONCPP_STRING lErr;
    Json::Value lRoot;

    if (false == lReader->parse(lStrBuffer.c_str(), lStrBuffer.c_str() + _fileSize, &lRoot, &lErr))
        return false;


    //암,복호화
    XORUtil lXORUtil;
    Base64Util lBase64;

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

        const auto& lMainListenerInfo = lRoot["MainListener"];

        //MainListener가 없을 경우 종료
        if (true == lMainListenerInfo.isNull())
            return false;

        m_oMainListenInfo.m_sServiceName = lMainListenerInfo.get("Name", "Server").asString();
        m_oMainListenInfo.m_nServerID = lMainListenerInfo.get("ServerID", 0).asInt();
        m_oMainListenInfo.m_sBindAddress = lMainListenerInfo.get("BindAddress", "127.0.0.1").asString();
        m_oMainListenInfo.m_nBindPort = lMainListenerInfo.get("BindPort", 0).asInt();
        m_oMainListenInfo.m_nTimeoutMS = lMainListenerInfo.get("TimeoutMS", DEFAULT_TIMEOUT_MS).asInt();
        m_oMainListenInfo.m_nMaxConnection = lMainListenerInfo.get("MaxConnection", DEFAULT_CONNECTION_COUNT).asInt();
        m_oMainListenInfo.m_nControlConnection = lMainListenerInfo.get("ControlConnection", DEFAULT_CONNECTION_CONTROL_COUNT).asInt();
        m_oMainListenInfo.m_nSendContextLimitCount = lMainListenerInfo.get("SendContextLimitCount", DEFAULT_SEND_CONTEXT_LIMIT_COUNT).asInt();
        m_oMainListenInfo.m_sServiceType = lMainListenerInfo.get("ServiceType", "Live").asString();

        if (m_oMainListenInfo.m_sServiceType == "Live")
        {
            m_oMainListenInfo.m_bIsLive = true;
        }
        else
        {
            m_oMainListenInfo.m_bIsLive = false;
        }

        m_oMainListenInfo.m_nClientVer = lMainListenerInfo.get("ClientVer", 0).asInt();

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

        m_oMainListenInfo.m_bIsMaintenanceMode = lMainListenerInfo.get("MaintenanceMode", false).asBool();
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
    if (const auto& lSubListen = lRoot["GameServerList"]; false == lSubListen.isNull())
    {
        for (const auto& lSubName : lSubListen.getMemberNames())
        {
            const auto& lSubListenInfo = lSubListen[lSubName];

            if (true == lSubListenInfo.isNull())
                continue;

            ServerListenerInfo lInsertInfo;
            lInsertInfo.m_nServerGroupID = lSubListenInfo.get("ServerGroupID", 0).asInt();
            lInsertInfo.m_nServerID = lSubListenInfo.get("ServerID", 0).asInt();
            lInsertInfo.m_sBindAddress = lSubListenInfo.get("BindAddress", 0).asString();
            lInsertInfo.m_nBindPort = lSubListenInfo.get("BindPort", 0).asInt();

            lInsertInfo.m_sPublicHost = lSubListenInfo.get("PublicHost", 0).asString();
            lInsertInfo.m_nPublicPort = lSubListenInfo.get("PublicPort", 0).asInt();

            if (lInsertInfo.m_sPublicHost.empty() || lInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'GameServerList' Info" << std::endl;
                continue;
            }

            m_umGameServers.insert(std::pair(lSubName, lInsertInfo));
            m_umServerTypeByServerID.insert_or_assign(lInsertInfo.m_nServerID, EServer::Game);
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
    if (const auto& lSubListen = lRoot["LoginServerList"]; false == lSubListen.isNull())
    {
        for (const auto& lSubName : lSubListen.getMemberNames())
        {
            const auto& lInfo = lSubListen[lSubName];
            if (true == lInfo.isNull())
                continue;

            ServerListenerInfo lInsertInfo;
            lInsertInfo.m_nServerID = lInfo.get("ServerID", 0).asInt();
            lInsertInfo.m_sBindAddress = lInfo.get("IP", "127.0.0.1").asString();
            lInsertInfo.m_nBindPort = lInfo.get("Port", 0).asInt();

            lInsertInfo.m_sPublicHost = lInfo.get("PublicHost", 0).asString();
            lInsertInfo.m_nPublicPort = lInfo.get("PublicPort", 0).asInt();

            if (lInsertInfo.m_sPublicHost.empty() || lInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'LoginServerList' Info" << std::endl;
                continue;
            }

            m_umLoginServers.insert(std::pair(lSubName, lInsertInfo));

            m_umServerTypeByServerID.insert_or_assign(lInsertInfo.m_nServerID, EServer::Login);
        }

    }

    //MessengerServer List
    m_umMessengerServers.clear();
    if (const auto& lSubListen = lRoot["MessengerServerList"]; false == lSubListen.isNull())
    {
        for (const auto& lSubName : lSubListen.getMemberNames())
        {
            const auto& lInfo = lSubListen[lSubName];
            if (true == lInfo.isNull())
                continue;

            ServerListenerInfo lInsertInfo;
            lInsertInfo.m_nServerGroupID = lInfo.get("ServerGroupID", 0).asInt();
            lInsertInfo.m_nServerID = lInfo.get("ServerID", 0).asInt();
            lInsertInfo.m_sBindAddress = lInfo.get("IP", "127.0.0.1").asString();
            lInsertInfo.m_nBindPort = lInfo.get("Port", 0).asInt();

            lInsertInfo.m_sPublicHost = lInfo.get("PublicHost", 0).asString();
            lInsertInfo.m_nPublicPort = lInfo.get("PublicPort", 0).asInt();

            if (lInsertInfo.m_sPublicHost.empty() || lInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'MessengerServer' Info" << std::endl;
                continue;
            }

            m_umMessengerServers.insert(std::pair(lSubName, lInsertInfo));

            m_umServerTypeByServerID.insert_or_assign(lInsertInfo.m_nServerID, EServer::Messenger);
        }
    }

    //LogServerList Start
    m_umLogServers.clear();
    if (const auto& lSubListen = lRoot["LogServerList"]; false == lSubListen.isNull())
    {
        for (const auto& lSubName : lSubListen.getMemberNames())
        {
            const auto& lInfo = lSubListen[lSubName];
            if (true == lInfo.isNull())
                continue;

            ServerListenerInfo lInsertInfo;
            lInsertInfo.m_nServerID = lInfo.get("ServerID", 0).asInt();
            lInsertInfo.m_sBindAddress = lInfo.get("IP", "127.0.0.1").asString();
            lInsertInfo.m_nBindPort = lInfo.get("Port", 0).asInt();

            if (lInsertInfo.m_sPublicHost.empty() || lInsertInfo.m_nPublicPort == 0)
            {
                std::cout << "Wrong Server Config. Check 'LogServerList' Info" << std::endl;
                continue;
            }

            m_umLogServers.insert(std::pair(lSubName, lInsertInfo));

            auto lFindIter = m_umServerTypeByServerID.find(lInsertInfo.m_nServerID);

            if (lFindIter == m_umServerTypeByServerID.end())
            {
                std::cout << "Check LogServer Config! LogServerID (" << lSubName << ") not matched GameServer" << std::endl;
                return false;
            }

            if (lFindIter->second != EServer::Game)
            {
                std::cout << "Check LogServer Config! LogServerID (" << lSubName << ") not matched GameServer" << std::endl;
                return false;
            }


            m_umServerTypeByServerID.insert_or_assign(lInsertInfo.m_nServerID, EServer::Log);
        }
    }
    //LogServerList End

    //Sub Listener
    m_umSubListenInfo.clear();
    if (const auto& lSubListen = lRoot["SubListener"]; false == lSubListen.isNull())
    {
        for (const auto& lSubName : lSubListen.getMemberNames())
        {
            const auto& lSubListenInfo = lSubListen[lSubName];
            if (true == lSubListenInfo.isNull())
                continue;

            ServerListenerInfo lInsertInfo;
            lInsertInfo.m_sBindAddress = lSubListenInfo.get("BindAddress", 0).asString();
            lInsertInfo.m_nBindPort = lSubListenInfo.get("BindPort", 0).asInt();

            m_umSubListenInfo.insert(std::pair(lSubName, lInsertInfo));
        }
    }
    //end Sub Listener

    //Connector
    m_umConnectorInfo.clear();
    if (const auto& lConnector = lRoot["Connect"]; false == lConnector.isNull())
    {
        for (const auto& lSubName : lConnector.getMemberNames())
        {
            const auto& lConnectorInfo = lConnector[lSubName];
            if (true == lConnectorInfo.isNull())
                continue;

            ConnectorInfo lInsertInfo;
            lInsertInfo.m_sBindAddress = lConnectorInfo.get("Host", 0).asString();
            lInsertInfo.m_nBindPort = lConnectorInfo.get("Port", 0).asInt();

        }
    }

    //DB Integrated
    m_umDBConnectionInfo.clear();
    if (const auto& lDB = lRoot["DB"]; false == lDB.isNull())
    {
        for (const auto& lName : lDB.getMemberNames())
        {
            const auto& lDBInfo = lDB[lName];
            if (true == lDBInfo.isNull())
                continue;

            DBInfo lInertInfo;
            lInertInfo.m_sDBHost = lDBInfo.get("Host", "127.0.0.1").asString();
            lInertInfo.m_nDBPort = lDBInfo.get("Port", 1433).asInt();
            lInertInfo.m_sDBName = lDBInfo.get("DATABASE", "").asString();

            lInertInfo.m_sUserID = lDBInfo.get("UID", "server").asString();
            lInertInfo.m_nThreadCount = lDBInfo.get("Thread", 0).asInt();

            std::string lTmpPwd = lDBInfo.get("Password", "").asString();

            char lTmp[1024] = { 0, };
            char lTmpForXOR[1024] = { 0, };

            size_t lDecodeSize = lBase64.Decode(lTmpPwd, lTmp, 1024);

            if (true == lXORUtil.Encrypt(lTmp, lDecodeSize, lTmpForXOR, 1024))
            {
                lInertInfo.m_sPassword = std::string(lTmpForXOR);
            }

            m_umDBConnectionInfo.insert(std::pair(lName, lInertInfo));
        }
    }
    // end DataBase

    // GDB for MessengerServer
    m_umGDBConnectionInfo.clear();
    if (const auto& lDB = lRoot["GDB"]; false == lDB.isNull())
    {
        for (const auto& lName : lDB.getMemberNames())
        {
            const auto& lDBInfo = lDB[lName];
            if (true == lDBInfo.isNull())
                continue;
            DBInfo lInertInfo;
            lInertInfo.m_sDBHost = lDBInfo.get("Host", "127.0.0.1").asString();
            lInertInfo.m_nDBPort = lDBInfo.get("Port", 1433).asInt();
            lInertInfo.m_sDBName = lDBInfo.get("DATABASE", "").asString();

            lInertInfo.m_sUserID = lDBInfo.get("UID", "server").asString();
            lInertInfo.m_nThreadCount = lDBInfo.get("Thread", 0).asInt();

            std::string lTmpPwd = lDBInfo.get("Password", "").asString();

            char lTmp[1024] = { 0, };
            char lTmpForXOR[1024] = { 0, };

            size_t lDecodeSize = lBase64.Decode(lTmpPwd, lTmp, 1024);

            if (true == lXORUtil.Encrypt(lTmp, lDecodeSize, lTmpForXOR, 1024))
            {
                lInertInfo.m_sPassword = std::string(lTmpForXOR);
            }

            m_umGDBConnectionInfo.insert(std::pair(stoi(lName), lInertInfo));

            int lServerID = std::atoi(lName.c_str());
            auto lFindIter = m_umServerTypeByServerID.find(lServerID);

            if (lFindIter == m_umServerTypeByServerID.end())
            {
                std::cout << "Check GDB Config! ServerID (" << lName << ") not matched GameServer" << std::endl;
                return false;
            }

            if (lFindIter->second != EServer::Game)
            {
                std::cout << "Check GDB Config! ServerID (" << lName << ") not matched GameServer" << std::endl;
                return false;
            }
        }
    }
    // GDB for MessengerServer End

    //LDB
    m_umLDBConnectionInfo.clear();
    if (const auto& lDB = lRoot["LDB"]; false == lDB.isNull())
    {
        for (const auto& lName : lDB.getMemberNames())
        {
            const auto& lDBInfo = lDB[lName];
            if (true == lDBInfo.isNull())
                continue;

            DBInfo lInertInfo;
            lInertInfo.m_sDBHost = lDBInfo.get("Host", "127.0.0.1").asString();
            lInertInfo.m_nDBPort = lDBInfo.get("Port", 1433).asInt();
            lInertInfo.m_sDBName = lDBInfo.get("DATABASE", "").asString();

            lInertInfo.m_sUserID = lDBInfo.get("UID", "server").asString();
            lInertInfo.m_nThreadCount = lDBInfo.get("Thread", 0).asInt();

            std::string lTmpPwd = lDBInfo.get("Password", "").asString();

            char lTmp[1024] = { 0, };
            char lTmpForXOR[1024] = { 0, };

            size_t lDecodeSize = lBase64.Decode(lTmpPwd, lTmp, 1024);

            if (true == lXORUtil.Encrypt(lTmp, lDecodeSize, lTmpForXOR, 1024))
            {
                lInertInfo.m_sPassword = std::string(lTmpForXOR);
            }

            m_umLDBConnectionInfo.insert(std::pair(stoi(lName), lInertInfo));
        }
    }
    // end LDB

    //ObjectPool
    m_umObjectPoolInfo.clear();
    if (const auto& lPool = lRoot["ObjectPools"]; false == lPool.isNull())
    {
        for (const auto& lName : lPool.getMemberNames())
        {
            const auto& lPoolInfo = lPool[lName];
            if (true == lPoolInfo.isNull())
                continue;

            int lVal = lPool.get(lName, 0).asInt();

            m_umObjectPoolInfo.insert(std::pair(lName, lVal));
        }
    }


    //Protocol Checker
    if (const auto& lProtocolCheck = lRoot["ProtocolChecker"]; false == lProtocolCheck.isNull())
    {
        m_bProtocolCheck = lProtocolCheck.get("Enable", false).asBool();
    }

    //TracePacket
    if (const auto& lTracePacket = lRoot["TracePacket"]; false == lTracePacket.isNull())
    {
        m_bTracePacket = lTracePacket.get("Enable", false).asBool();
        m_bTracePacketIgnoreBattle = lTracePacket.get("IgnoreBattle", false).asBool();
    }

    //Use MDB
    if (const auto& lMDB = lRoot["MDB"]; false == lMDB.isNull())
    {
        m_bUseMDB = lMDB.get("Enable", false).asBool();
        m_sMDBPath = lMDB.get("Path", "").asString();
    }

    //log
    if (const auto& lLog = lRoot["Log"]; false == lLog.isNull())
    {
        m_sLogDir = lLog.get("Dir", "../Log/").asString();
        m_nLogLvl = lLog.get("Level", 0).asInt();
    }


    //Map
    m_sMapInfoDir = lRoot.get("MapDir", "").asString();

    //DumpDir
    m_sDumpDir = lRoot.get("DumpDir", "").asString();
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
    SYSTEM_INFO lInfo;
    GetSystemInfo(&lInfo);

    m_nProcessCount = (int)lInfo.dwNumberOfProcessors;

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
    int lIdx = (m_nSwitch.load() + 1) % CONFIG_SWITCH_SIZE;

    ServerConfigData lLoadData;
    if (false == lLoadData.LoadConfig(m_sConfigFileName))
        return false;

    std::swap(m_oServerConfigData[lIdx], lLoadData);

    m_nSwitch.store(lIdx);
    return true;
}

ServerConfigData& ServerConfig::GetConfig()
{
    int lIdx = m_nSwitch.load() % CONFIG_SWITCH_SIZE;
    return m_oServerConfigData[lIdx];
}

int ServerConfig::GetServerID()
{
    return ServerConfig::GetInst().GetConfig().GetMainListenerInfo().m_nServerID;
}

bool ServerConfig::IsMessengerServer()
{
    return ServerConfig::GetInst().GetConfig().GetServerType() == EServer::Type::Messenger ? true : false;
}
