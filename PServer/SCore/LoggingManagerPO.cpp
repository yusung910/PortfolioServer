#include "stdafx.hxx"
#include "LoggingManagerPO.hxx"
#include <iostream>
#include <fstream>
#include <syncstream>

LoggingManagerPO::LoggingManagerPO()
{
    //시간
    tm localCurrentTime = {};
    //틱
    time_t localCurrentTick = time(nullptr);

    //서버 로컬 타임 호출
    auto localErr = localtime_s(&localCurrentTime, &localCurrentTick);

    //서버 로컬 타임을 정상적으로 호출 했을 경우
    if (localErr == 0)
    {
        //로그 파일을 저장할 폴더명을 생성
        char localTmp[7] = { 0, };
        sprintf_s(localTmp, sizeof(localTmp), "%02d%02d%02d", (localCurrentTime.tm_year + 1900) % 100, localCurrentTime.tm_mon + 1, localCurrentTime.tm_mday);

        m_sLogDateFolder = localTmp;
    }
    else
    {
        m_sLogDateFolder = "000000";
    }
}

LoggingManagerPO::~LoggingManagerPO()
{

}

bool LoggingManagerPO::SetLogDirConfig(const std::string& _path) noexcept
{
    if (true == _path.empty()) return false;
    m_oLogDirPath.assign(_path.begin(), _path.end());
    return true;
}

bool LoggingManagerPO::SetServerName(const std::string& _name) noexcept
{
    if (true == _name.empty()) return false;
    m_sServerName.assign(_name.begin(), _name.end());
    return true;
}

bool LoggingManagerPO::SetServerNum(const int& _num) noexcept
{
    if (_num < 0) return false;
    m_nServerNum = _num;
    return true;
}

bool LoggingManagerPO::Start()
{
    if (true == m_bIsRunning.load()) return false;
    if (nullptr != m_pThread) return false;

    // 로그 저장 폴더 경로 설정
    std::string localTmpDir = m_sLogDirConfig + "/" + m_sLogDateFolder + "/";
    m_oLogDirPath = std::filesystem::path(localTmpDir.c_str());

    if (false == _MakeFolder()) return false;

    _ChangeInfoFile();
    _ChangeErrorFile();

    m_bIsRunning.store(true);

    m_pThread = new std::thread(&LoggingManagerPO::_Run, this);

    return true;
}

bool LoggingManagerPO::Stop()
{
    if (false == m_bIsRunning.load()) return false;

    m_bIsRunning.store(false);
    if (m_pThread->joinable()) m_pThread->join();

    SafeDelete(m_pThread);

    return true;
}

void LoggingManagerPO::Log(const std::wstring& _msg, const ELogLevel& _lvl, const bool& _showConsole, const bool& _writeFile)
{
    if (false == m_bIsRunning.load())
        return;

#ifndef _DEBUG
    if (_lvl == ELogLevel::Debug)
        return;
#endif // !_DEBUG

    m_oLogData.Push(new LogData(_lvl, _msg, _showConsole, _writeFile));
}

void LoggingManagerPO::Log(const std::string& _msg, const ELogLevel& _lvl, const bool& _showConsole, const bool& _writeFile)
{
    if (false == m_bIsRunning.load())
        return;

#ifndef _DEBUG
    if (_lvl == ELogLevel::Debug)
        return;
#endif // !_DEBUG

    m_oLogData.Push(new LogData(_lvl, _msg, _showConsole, _writeFile));
}

void LoggingManagerPO::_Run()
{
    if (nullptr == m_pThread)
        return;

}

bool LoggingManagerPO::_MakeFolder()
{
    //로그를 저장할 경로가 존재하고 있는지 확인
    if (true == std::filesystem::exists(m_oLogDirPath))
    {
        //로그를 저장할 경로가 존재 할 경우 해당 경로가 디렉토리인지 확인
        if (true == std::filesystem::is_directory(m_oLogDirPath))
            return true;
    }

    std::error_code localEC = {};
    //디렉토리 생성
    if (false == std::filesystem::create_directories(m_oLogDirPath, localEC))
    {
        if (localEC.value() == 0)
            return true;

        return false;
    }

    return true;
}

bool LoggingManagerPO::_CheckInfoFileChangeable()
{
    return _CheckFileChangeable(m_oLogInfoFilePath);
}

bool LoggingManagerPO::_CheckErrorFileChangeable()
{
    return _CheckFileChangeable(m_oLogErrorFilePath);
}

bool LoggingManagerPO::_CheckFileChangeable(const std::filesystem::path& _file)
{
    // 파일이 존재 하는지 여부 확인
    if (false == std::filesystem::exists(_file))
        return false;

    auto localSize = std::filesystem::file_size(_file);
    if (localSize > MAX_LOG_FILE_SIZE)
        return true;

    return false;
}

bool LoggingManagerPO::_ChangeInfoFile()
{
    tm localCurrentTime = {};
    time_t localCurrentTick = time(nullptr);
    [[maybe_unused]] auto localErr = localtime_s(&localCurrentTime, &localCurrentTick);

    char localTmpPath[MAX_PATH] = { 0, };
    sprintf_s(localTmpPath, sizeof(localTmpPath), "%s/%s/%s.%d_%04d%02d%02d_%02d%02d%02d.log",
        m_sLogDirConfig.c_str(),
        m_sLogDateFolder.c_str(),
        m_sServerName.c_str(),
        m_nServerNum,
        localCurrentTime.tm_year + 1900,
        localCurrentTime.tm_mon + 1,
        localCurrentTime.tm_mday,
        localCurrentTime.tm_hour,
        localCurrentTime.tm_min,
        localCurrentTime.tm_sec
    );

    std::filesystem::path checkPath(localTmpPath);
    if (true == std::filesystem::exists(checkPath))
        return false;

    m_oLogInfoFilePath = checkPath;
    return true;
}

bool LoggingManagerPO::_ChangeErrorFile()
{
    tm localCurrentTime = {};
    time_t localCurrentTick = time(nullptr);
    [[maybe_unused]] auto localErr = localtime_s(&localCurrentTime, &localCurrentTick);

    char localTmpPath[MAX_PATH] = { 0, };
    sprintf_s(localTmpPath, sizeof(localTmpPath), "%s/%s/%s.%d_%04d%02d%02d_%02d%02d%02d.log",
        m_sLogDirConfig.c_str(),
        m_sLogDateFolder.c_str(),
        m_sServerName.c_str(),
        m_nServerNum,
        localCurrentTime.tm_year + 1900,
        localCurrentTime.tm_mon + 1,
        localCurrentTime.tm_mday,
        localCurrentTime.tm_hour,
        localCurrentTime.tm_min,
        localCurrentTime.tm_sec
    );

    std::filesystem::path checkPath(localTmpPath);
    if (true == std::filesystem::exists(checkPath))
        return false;

    m_oLogInfoFilePath = checkPath;
    return true;
}

void LoggingManagerPO::_ChangeConsoleColor(const ELogLevel& _lvl)
{
    WORD localColor = 7;    //LIGHT GRAY

    switch (_lvl)
    {
    case ELogLevel::Info:
        localColor = 7;
        break;
    case ELogLevel::Warning:
        localColor = 14; // Yellow
        break;
    case ELogLevel::Error:
        localColor = 4; // Red
        break;
    case ELogLevel::System:
        localColor = 10; // LIGHT GREEN
        break;
    case ELogLevel::Debug:
        localColor = 9; // Light Blue
        break;
    default:
        break;
    }

    if (localColor == m_nLastConsoleColor)
        return;

    m_nLastConsoleColor = localColor;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), localColor);
}
