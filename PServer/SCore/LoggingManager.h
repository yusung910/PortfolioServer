#pragma once

#include "SCoreAPI.h"
#include "LoggingCommon.h"
#include <string>
#include <RefSingleton.h>

class LoggingManagerPO;

class SCoreAPI LoggingManager : public RefSingleton<LoggingManager>
{
private:
    LoggingManagerPO* m_pManagerPO = nullptr;

public:
    LoggingManager();
    virtual ~LoggingManager();

    virtual void Initialize();

    //Set
    bool SetLogDirConfig(const std::string& _path = DEFAULT_LOG_PATH) noexcept;
    bool SetServerName(const std::string& _name = DEFAULT_SERVER_NAME) noexcept;
    bool SetServerNum(const int& _num = DEFAULT_SERVER_NUMBER) noexcept;

    bool Start();
    bool Stop();

    void ViewForce(const char* _pFunc, const ELogLevel& _lvl, const wchar_t* _fmt, ...);
    void ViewForce(const char* _pFunc, const ELogLevel& _lvl, const char* _fmt, ...);

    void Log(const char* _pFunc, const ELogLevel& _lvl, const bool& _view, const bool& _write, const wchar_t* _fmt, ...);
    void Log(const char* _pFunc, const ELogLevel& _lvl, const bool& _view, const bool& _write, const char* _fmt, ...);


private:
    std::wstring _GetFunctionString(const char* _pFunc, const ELogLevel& _lvl);
    std::string _GetFunctionStringA(const char* _pFunc, const ELogLevel& _lvl);

    std::wstring _GetLineString(int _line, const ELogLevel& _lvl);
    std::string _GetLineStringA(int _line, const ELogLevel& _lvl);
};

