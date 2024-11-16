/**
 *  @file ServerConfig.h
 *  @author YS
 *  @date 2024-11-15
 *  @project PGPublic
 *
 *  ���� ���࿡ �ʿ��� Ŭ������ ���ִ�
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
 *  ���� ����� Listener ������ �����ϴ� Ŭ����
 */
class ServerListenerInfo : public ListenerInfo
{
public:
    //���� �׷� ���̵�
    int m_nServerGroupID = 0;
    //���� �ĺ� ���̵�
    int m_nServerID = 0;
    //���� ������ �ʿ��� ������ ���� �ο�(���� �ӵ��� ���Ͻ�Ű�� ����)
    int m_nControlConnection = DEFAULT_CONNECTION_CONTROL_COUNT;
    //���� ������ Ŭ���̾�Ʈ���� ������ Context���� ��
    int m_nSendContextLimitCount = DEFAULT_SEND_CONTEXT_LIMIT_COUNT;


    std::string m_sServiceType = "";

    //Ŭ���̾�Ʈ���� �˸� ���� ����
    std::string m_sPublicHost = "";
    int m_nPublicPort = 0;
    int m_nClientVer = 0;

    bool m_bIsLive = false;

    EServer::Type m_eServerType = EServer::None;

    bool m_bIsMaintenanceMode = false; //���� �� ���̺� ���� ���� ���� ���θ� Ȯ��

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
     *  json���Ͽ� ����� serverconfig ������ Load�ϰ�
     *  ��� �� �� �ֵ��� ������ �Ľ��ϴ� �Լ�
     *
     *      @param [in] _confFile 
     *      @param [in] _fileSize 
     *
     *      @return 
     */
    bool _LoadConfig(const std::wstring& _confFile, size_t _fileSize);

    /*!
     *  Connection ���� unordered_map ���� �ʱ�ȭ
     */
    void _Clear();

    /*!
     *  ���� ��ǻ���� CPU �ھ� ������ ȣ�� GetSystemInfo()
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
