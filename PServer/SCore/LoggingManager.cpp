#include "stdafx.hxx"
#include "LoggingManager.h"
#include "LoggingManagerPO.hxx"
#include "StringUtil.h"

// 스택 사이즈 오류
// 메모리의 스택영역에 저장되는 데이터의 크기가 일정 수치 넘어갈 경우
// C6262 경고 메세지(에러 X)를 출력한다. 이 경고 메세지를 해결하기 위해
// 해당 데이터를 힙 영역에 저장해야 한다
// 힙 영역에 저장하기 위해 동적할당을 이용한다.
#define LOG_C6262_TUNE

LoggingManager::LoggingManager()
{
}

LoggingManager::~LoggingManager()
{
    SafeDelete(m_pManagerPO);
}

void LoggingManager::Initialize()
{
    std::setlocale(LC_ALL, "ko_KR.UTF-8");
    m_pManagerPO = new LoggingManagerPO();
}

bool LoggingManager::SetLogDirConfig(const std::string& _path) noexcept
{
    if(m_pManagerPO == nullptr)
        return false;

    return m_pManagerPO->SetLogDirConfig(_path);
}

bool LoggingManager::SetServerName(const std::string& _name) noexcept
{
    if (m_pManagerPO == nullptr)
        return false;

    return m_pManagerPO->SetServerName(_name);
}

bool LoggingManager::SetServerNum(const int& _num) noexcept
{
    if (m_pManagerPO == nullptr)
        return false;

    return m_pManagerPO->SetServerNum(_num);
}

bool LoggingManager::Start()
{
    if (m_pManagerPO == nullptr)
        return false;
    return m_pManagerPO->Start();
}

bool LoggingManager::Stop()
{
    if (m_pManagerPO == nullptr)
        return false;
    return m_pManagerPO->Stop();
}

void LoggingManager::ViewForce(const char* _pFunc, const ELogLevel& _lvl, const wchar_t* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return ;

    switch (_lvl)
    {
    case ELogLevel::System:
    case ELogLevel::Info:
        return;
    default:
        break;
    }

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<wchar_t> localpBuf(new wchar_t[MAX_LOG_STRING_SIZE], std::default_delete<wchar_t[]>());
    wchar_t* localpBuffer = localpBuf.get();
#else
    wchar_t localpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list localArgs;
    va_start(localArgs, _fmt);
    _vsnwprintf_s(localpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, localArgs);
    va_end(localArgs);

    std::wstring localStrRet = _GetFunctionString(_pFunc, _lvl);
    localStrRet.append(localpBuffer);
    m_pManagerPO->ViewForce(localStrRet, _lvl);

}

void LoggingManager::ViewForce(const char* _pFunc, const ELogLevel& _lvl, const char* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return;

    switch (_lvl)
    {
    case ELogLevel::System:
    case ELogLevel::Info:
        return;
    default:
        break;
    }

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<char> localpBuf(new char[MAX_LOG_STRING_SIZE], std::default_delete<char[]>());
    char* localpBuffer = localpBuf.get();
#else
    char localpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list localArgs;
    va_start(localArgs, _fmt);
    _vsnprintf_s(localpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, localArgs);
    va_end(localArgs);

    std::string localStrRet = _GetFunctionStringA(_pFunc, _lvl);
    localStrRet.append(localpBuffer);
    m_pManagerPO->ViewForce(localStrRet, _lvl);
}

void LoggingManager::Log(const char* _pFunc, const ELogLevel& _lvl, const bool& _view, const bool& _write, const wchar_t* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return;

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<wchar_t> localpBuf(new wchar_t[MAX_LOG_STRING_SIZE], std::default_delete<wchar_t[]>());
    wchar_t* localpBuffer = localpBuf.get();
#else
    wchar_t localpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list localArgs;
    va_start(localArgs, _fmt);
    _vsnwprintf_s(localpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, localArgs);
    va_end(localArgs);

    std::wstring localStrRet = _GetFunctionString(_pFunc, _lvl);
    localStrRet.append(localpBuffer);
    m_pManagerPO->Log(localStrRet, _lvl, _view, _write);
}

void LoggingManager::Log(const char* _pFunc, const ELogLevel& _lvl, const bool& _view, const bool& _write, const char* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return;

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<char> localpBuf(new char[MAX_LOG_STRING_SIZE], std::default_delete<char[]>());
    char* localpBuffer = localpBuf.get();
#else
    char localpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list localArgs;
    va_start(localArgs, _fmt);
    _vsnprintf_s(localpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, localArgs);
    va_end(localArgs);

    std::string localStrRet = _GetFunctionStringA(_pFunc, _lvl);
    localStrRet.append(localpBuffer);
    m_pManagerPO->Log(localStrRet, _lvl, _view, _write);
}

std::wstring LoggingManager::_GetFunctionString(const char* _pFunc, const ELogLevel& _lvl)
{
    switch (_lvl)
    {
    case ELogLevel::System:
    case ELogLevel::Info:
        return L"";
    default:
        break;
    }

    if (nullptr == _pFunc)
        return L"";

    wchar_t localTmp[MAX_PATH];
    swprintf_s(localTmp, MAX_PATH, L"on\'%s\' ", StringUtil::ToWideChar(_pFunc).c_str());

    return std::wstring(localTmp);
}

std::string LoggingManager::_GetFunctionStringA(const char* _pFunc, const ELogLevel& _lvl)
{
    switch (_lvl)
    {
    case ELogLevel::System:
    case ELogLevel::Info:
        return "";
    default:
        break;
    }

    if (nullptr == _pFunc)
        return "";

    char localTmp[MAX_PATH];
    sprintf_s(localTmp, MAX_PATH, "on\'%s\' ", _pFunc);

    return std::string(localTmp);
}

std::wstring LoggingManager::_GetLineString(int _line, const ELogLevel& _lvl)
{
    switch (_lvl)
    {
    case ELogLevel::System:
    case ELogLevel::Info:
        return L"";
    default:
        break;
    }


    wchar_t localTmp[MAX_PATH];
    swprintf_s(localTmp, MAX_PATH, L"[line]:\'%d\' ", _line);

    return std::wstring(localTmp);
}

std::string LoggingManager::_GetLineStringA(int _line, const ELogLevel& _lvl)
{
    switch (_lvl)
    {
    case ELogLevel::System:
    case ELogLevel::Info:
        return "";
    default:
        break;
    }


    char localTmp[MAX_PATH];
    sprintf_s(localTmp, MAX_PATH, "[line]:\'%d\' ", _line);

    return std::string(localTmp);
}
