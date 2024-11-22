#pragma once
#include "LoggingCommon.h"

#include <string>
#include <chrono>

class LogData
{
public:
    ELogLevel m_eLogLvl = ELogLevel::None;
    std::wstring m_sLogMsg = L"";
    std::chrono::system_clock::time_point m_oLogTime = {};
    bool m_bShowConsole = false;
    bool m_bWriteFile = false;

public:
    LogData();
    LogData(const ELogLevel& _lvl, const std::string& _msg, const bool& _showConsole, const bool& _writeFile);
    LogData(const ELogLevel& _lvl, const std::wstring& _msg, const bool& _showConsole, const bool& _writeFile);

    ~LogData() = default;

    void SetData(const ELogLevel& _lvl, const std::string& _msg, const bool& _showConsole, const bool& _writeFile);
    void SetData(const ELogLevel& _lvl, const std::wstring& _msg, const bool& _showConsole, const bool& _writeFile);

    std::wstring ToString();
};

