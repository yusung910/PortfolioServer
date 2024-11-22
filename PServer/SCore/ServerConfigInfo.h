#pragma once
#include <string>

constexpr int DEFAULT_TIMEOUT_MS = 3600000;

//�⺻ �ִ� ���� ���� ��
constexpr int DEFAULT_MAX_CONNECTION = 5000;

// ������ ���� ���� ���� ����
struct ListenerInfo
{
    std::string m_sServiceName = "Game";

    std::string m_sBindAddress = "0.0.0.0";
    int m_nBindPort = 0;
    int m_nMaxConnection = DEFAULT_MAX_CONNECTION;
    int m_nTimeoutMS = DEFAULT_TIMEOUT_MS;
};


// DB ���� ����
struct DBInfo
{
    std::string m_sDBHost = "127.0.0.1";
    int m_nDBPort = 1433;
    std::string m_sUserID = "";
    std::string m_sPassword = "";
    std::string m_sDBName = "";

    int m_nThreadCount = 1;

    bool IsValid()
    {
        if (true == m_sDBHost.empty()
            || true == m_sUserID.empty()
            || true == m_sDBName.empty()
            || 0 >= m_nThreadCount
            || 0 >= m_nDBPort
            || 0x10000 <= m_nDBPort
            )
            return false;
        return true;
    }
};

constexpr const char* DEFAULT_SERVICE_NAME = "Server";
constexpr const char* DEFAULT_LOG_DIR = "./Log/";

//�α� ��� ���� ����
struct LogSettingInfo
{
    std::string m_sServiceName = DEFAULT_SERVICE_NAME;
    std::string m_sLogDir = DEFAULT_LOG_DIR;

    int m_nServerID = 0;
};
