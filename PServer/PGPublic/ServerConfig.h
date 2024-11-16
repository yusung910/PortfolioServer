/**
 *  @file ServerConfig.h
 *  @author YS
 *  @date 2024-11-15
 *  @project PGPublic
 *
 *  서버 실행에 필요한 클래스가 모여있다
 */
#pragma once
#include <string>
#include <unordered_map>
#include <RefSingleton.h>
#include "ELang.h"

#include <ServerConfigInfo.h>
#include <GlobalEnum.h>

constexpr int DEFAULT_CONNECTION_COUNT = 7000;
constexpr int DEFAULT_CONNECTION_CONTROL_COUNT = 6000;
constexpr int DEFAULT_SEND_CONTEXT_LIMIT_COUNT = 500;


/*!
 *  서버 실행시 Listener 정보를 관리하는 클래스
 */
class ServerListenerInfo : public ListenerInfo
{
public:
    //서버 그룹 아이디
    int m_nServerGroupID = 0;
    //서버 식별 아이디
    int m_nServerID = 0;
    //접속 통제가 필요한 시점의 기준 인원(접속 속도를 저하시키는 시점)
    int m_nControlConnection = DEFAULT_CONNECTION_CONTROL_COUNT;
    //서버 접속한 클라이언트에게 보내는 Context수가 이
    int m_nSendContextLimitCount = DEFAULT_SEND_CONTEXT_LIMIT_COUNT;


    std::string m_sServiceType = "";

    //클라이언트에게 알릴 접속 정보
    std::string m_sPublicHost = "";
    int m_nPublicPort = 0;
    int m_nClientVer = 0;

    bool m_bIsLive = false;

    EServer::Type m_eServerType = EServer::None;

    bool m_bIsMaintenanceMode = false; //점검 중 라이브 빌드 접속 가능 여부를 확인

};

using ConnectorInfo = ServerListenerInfo;

constexpr int CONFIG_SWITCH_SIZE = 2;

class ServerConfigData
{
private:
    //Listener
    ServerListenerInfo m_oMainListenInfo;
    std::unordered_map<std::string, ServerListenerInfo> m_umSubListenInfo;

    //Connector
    std::unordered_map<std::string, ConnectorInfo> m_umConnectorInfo;
    
    //-------------------------------------------------------------------
    //DB
    //-------------------------------------------------------------------
    std::unordered_map<std::string, DBInfo> m_umDBConnectionInfo;
    //GameDB
    std::unordered_map<int, DBInfo> m_umGDBConnectionInfo;
    //LogDB
    std::unordered_map<int, DBInfo> m_umLDBConnectionInfo;
    //-------------------------------------------------------------------


    //Log
    std::string m_sLogDir = "";
    int m_nLogLvl = 0;

    //Dump
    std::string m_sDumpDir = "";

    //ObjectPool
    std::unordered_map<std::string, int> m_umObjectPoolInfo;

    //Map Info
    std::string m_sMapInfoDir = "";

    //
    bool m_bProtocolCheck = false;

    bool m_bTracePacket = false;
    bool m_bTracePacketIgnoreBattle = false;

    //MDB
    bool m_bUseMDB = false;
    std::string m_sMDBPath = "";

    bool m_bIsLoaded = false;

    //ProcessCount
    int m_nProcessCount = 1;

    //ServerList
    std::unordered_map<std::string, ConnectorInfo> m_umGameServers;
    std::unordered_map<std::string, ConnectorInfo> m_umLoginServers;
    std::unordered_map<std::string, ConnectorInfo> m_umMessengerServers;
    std::unordered_map<std::string, ConnectorInfo> m_umLogServers;


    //std::string m_sPaymentURL;
    //std::string m_sAPIServerURL;

    std::unordered_map<int, EServer::Type> m_umServerTypeByServerID;

    //Platform
    //std::string m_sGoogleClientID = "";
    //std::string m_sGooglePrivateFileName = "";
    //std::string m_sGooglePackageName = "";
    //std::string m_sGoogleAPIKey = "";

    //std::string m_sOneStoreID = "";
    //std::string m_sOneStoreClientID = "";
    //std::string m_sOneStoreKey = "";

    std::wstring m_sConfigFileName = L"";

    ELang m_eLanguage = ELang::KR;

public:
    bool LoadConfig(const std::wstring& _configFile = L"ServerConfig.json");
    bool Reload();

    const ServerListenerInfo& GetMainListenerInfo() const;
    const ServerListenerInfo* GetSubListenerInfo(const std::string& _subName) const;
    const ConnectorInfo* GetConnectorInfo(const std::string& _connectName) const;
    const DBInfo* GetDBConnectionInfo(const std::string& _dbTypeName) const;

    const std::unordered_map<int, DBInfo>& GetGDBConnectionInfo() const;
    const std::unordered_map<int, DBInfo>& GetLDBConnectionInfo() const;

    std::string GetLogDir() const;
    const int& GetLogLevel() const;
    std::string GetDumpDir() const;
    const int GetObjectPoolSize(const std::string& _strName) const;
    std::string GetMapInfoPath() const;

    const bool& GetTracePacket() const;
    const bool& GetTracePacketIgnoreBattle() const;

    const bool& UseMDB() const;
    std::string GetMDBPath() const;

    const bool& UseCheckProtocol() const;

    const int& GetCPUCount() const;

    bool IsLoaded() const;

    std::unordered_map<std::string, ConnectorInfo>& GetGameServerList();
    std::unordered_map<std::string, ConnectorInfo>& GetLoginServerList();
    std::unordered_map<std::string, ConnectorInfo>& GetMessengerServerList();
    std::unordered_map<std::string, ConnectorInfo>& GetLogServerList();
    std::string& GetAPIServerURL();

    bool GetMessengerServerInfo(int serverGroupID, ConnectorInfo& o_connectorInfo);

    const int& GetServerID() const;
    const int& GetUnityServerID() const;
    EServer::Type GetServerType() const;


    const int& GetClientVer() const;

    const ELang& GetLanguageType() const;

private:

    /*!
     *  json파일에 저장된 serverconfig 내용을 Load하고
     *  사용 할 수 있도록 변수에 파싱하는 함수
     *
     *      @param [in] _confFile 
     *      @param [in] _fileSize 
     *
     *      @return 
     */
    bool _LoadConfig(const std::wstring& _confFile, size_t _fileSize);

    /*!
     *  Connection 관련 unordered_map 변수 초기화
     */
    void _Clear();

    /*!
     *  서버 컴퓨터의 CPU 코어 갯수를 호출 GetSystemInfo()
     */
    void _LoadCPUCount();
};

class ServerConfig
{
private:
    ServerConfigData m_oServerConfigData[CONFIG_SWITCH_SIZE];
    std::atomic_int m_nSwitch = 0;
    std::atomic_bool m_bIsFirstLoaded = false;
    std::wstring m_sConfigFileName = L"";

public:
    bool LoadConfig(const std::wstring& _confFile = L"ServerConfig.json");
    bool ReloadConfig();

    ServerConfigData& GetConfig();

    static int GetServerID();

    static bool IsMessengerServer();


};
