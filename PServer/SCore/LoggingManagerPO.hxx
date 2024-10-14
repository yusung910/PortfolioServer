#pragma once

#include "LoggingCommon.h"
#include "LogData.hxx"

//
#include "BlockingQueue.h"

#include <thread>
#include <deque>
#include <atomic>
#include <filesystem>

constexpr WORD DEFAULT_CONSOLE_COLOR = 7;	// Light Gray

class LoggingManagerPO
{
private:
    BlockingQueue<LogData>  m_oLogDataQueue;
    std::atomic_bool m_bIsRunning = false;
    std::thread* m_pThread = nullptr;

    std::filesystem::path m_oLogDirPath = {};
    std::filesystem::path m_oLogInfoFilePath = {};
    std::filesystem::path m_oLogErrorFilePath = {};

    WORD m_nLastConsoleColor = DEFAULT_CONSOLE_COLOR;

    std::string m_sServerName = DEFAULT_SERVER_NAME;
    int m_nServerNum = DEFAULT_SERVER_NUMBER;

    std::string m_sLogDirConfig = DEFAULT_LOG_PATH; //로그 root dir
    std::string m_sLogDateFolder = "";  //로그 저장 폴더

public:
    LoggingManagerPO(); //Constructor
    ~LoggingManagerPO(); //Destructor

    //Set
    bool SetLogDirConfig(const std::string& _path = DEFAULT_LOG_PATH) noexcept;
    bool SetServerName(const std::string& _name = DEFAULT_SERVER_NAME) noexcept;
    bool SetServerNum(const int& _num = DEFAULT_SERVER_NUMBER) noexcept;

    bool Start();
    bool Stop();

    void Log(const std::wstring& _msg, const ELogLevel& _lvl, const bool& _showConsole, const bool& _writeFile);
    void Log(const std::string& _msg, const ELogLevel& _lvl, const bool& _showConsole, const bool& _writeFile);

    void ViewForce(const std::wstring& _log, const ELogLevel& _lvl);
    void ViewForce(const std::string& _log, const ELogLevel& _lvl);

private:
    void _Run();

    bool _MakeFolder();

    bool _CheckInfoFileChangeable();
    bool _CheckErrorFileChangeable();
    bool _CheckFileChangeable(const std::filesystem::path& _file);


    bool _ChangeInfoFile();
    bool _ChangeErrorFile();

    //콘솔 컬러 지정
    void _ChangeConsoleColor(const ELogLevel& _lvl);

};

