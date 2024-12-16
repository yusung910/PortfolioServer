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

void LoggingManager::Init()
{
    std::setle(LC_ALL, "ko_KR.UTF-8");
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
    std::shared_ptr<wchar_t> lpBuf(new wchar_t[MAX_LOG_STRING_SIZE], std::default_delete<wchar_t[]>());
    wchar_t* lpBuffer = lpBuf.get();
#else
    wchar_t lpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list lArgs;
    va_start(lArgs, _fmt);
    _vsnwprintf_s(lpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, lArgs);
    va_end(lArgs);

    std::wstring lStrRet = _GetFunctionString(_pFunc, _lvl);
    lStrRet.append(lpBuffer);
    m_pManagerPO->ViewForce(lStrRet, _lvl);

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
    std::shared_ptr<char> lpBuf(new char[MAX_LOG_STRING_SIZE], std::default_delete<char[]>());
    char* lpBuffer = lpBuf.get();
#else
    char lpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list lArgs;
    va_start(lArgs, _fmt);
    _vsnprintf_s(lpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, lArgs);
    va_end(lArgs);

    std::string lStrRet = _GetFunctionStringA(_pFunc, _lvl);
    lStrRet.append(lpBuffer);
    m_pManagerPO->ViewForce(lStrRet, _lvl);
}

void LoggingManager::Log(const char* _pFunc, const ELogLevel& _lvl, const bool& _view, const bool& _write, const wchar_t* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return;

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<wchar_t> lpBuf(new wchar_t[MAX_LOG_STRING_SIZE], std::default_delete<wchar_t[]>());
    wchar_t* lpBuffer = lpBuf.get();
#else
    wchar_t lpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list lArgs;
    va_start(lArgs, _fmt);
    _vsnwprintf_s(lpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, lArgs);
    va_end(lArgs);

    std::wstring lStrRet = _GetFunctionString(_pFunc, _lvl);
    lStrRet.append(lpBuffer);
    m_pManagerPO->Log(lStrRet, _lvl, _view, _write);
}

void LoggingManager::Log(const char* _pFunc, const ELogLevel& _lvl, const bool& _view, const bool& _write, const char* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return;

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<char> lpBuf(new char[MAX_LOG_STRING_SIZE], std::default_delete<char[]>());
    char* lpBuffer = lpBuf.get();
#else
    char lpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list lArgs;
    va_start(lArgs, _fmt);
    _vsnprintf_s(lpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, lArgs);
    va_end(lArgs);

    std::string lStrRet = _GetFunctionStringA(_pFunc, _lvl);
    lStrRet.append(lpBuffer);
    m_pManagerPO->Log(lStrRet, _lvl, _view, _write);
}

void LoggingManager::Log(const char* _pFunc, const int _line, const ELogLevel& _lvl, const bool& _view, const bool& _write, const wchar_t* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return;

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<wchar_t> lpBuf(new wchar_t[MAX_LOG_STRING_SIZE], std::default_delete<wchar_t[]>());
    wchar_t* lpBuffer = lpBuf.get();
#else
    wchar_t lpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list lArgs;
    va_start(lArgs, _fmt);
    _vsnwprintf_s(lpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, lArgs);
    va_end(lArgs);

    std::wstring lStrRet = _GetFunctionString(_pFunc, _lvl);
    lStrRet.append(_GetLineString(_line, _lvl));
    lStrRet.append(lpBuffer);
    m_pManagerPO->Log(lStrRet, _lvl, _view, _write);
}

void LoggingManager::Log(const char* _pFunc, const int _line, const ELogLevel& _lvl, const bool& _view, const bool& _write, const char* _fmt, ...)
{
    if (m_pManagerPO == nullptr) return;

#ifdef LOG_C6262_TUNE
    //unique_ptr, shared_ptr(포인터)가 delete될 때 실행 될 기본 Destructor(소멸자)를 명시한다
    std::shared_ptr<char> lpBuf(new char[MAX_LOG_STRING_SIZE], std::default_delete<char[]>());
    char* lpBuffer = lpBuf.get();
#else
    char lpBuffer[MAX_LOG_STRING_SIZE] = { 0, };
#endif

    va_list lArgs;
    va_start(lArgs, _fmt);
    _vsnprintf_s(lpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, lArgs);
    va_end(lArgs);

    std::string lStrRet = _GetFunctionStringA(_pFunc, _lvl);
    lStrRet.append(_GetLineStringA(_line, _lvl));
    lStrRet.append(lpBuffer);
    m_pManagerPO->Log(lStrRet, _lvl, _view, _write);
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

    wchar_t lTmp[MAX_PATH];
    swprintf_s(lTmp, MAX_PATH, L"on\'%s\' ", StringUtil::ToWideChar(_pFunc).c_str());

    return std::wstring(lTmp);
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

    char lTmp[MAX_PATH];
    sprintf_s(lTmp, MAX_PATH, "on\'%s\' ", _pFunc);

    return std::string(lTmp);
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


    wchar_t lTmp[MAX_PATH];
    swprintf_s(lTmp, MAX_PATH, L"[line]:\'%d\' ", _line);

    return std::wstring(lTmp);
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


    char lTmp[MAX_PATH];
    sprintf_s(lTmp, MAX_PATH, "[line]:\'%d\' ", _line);

    return std::string(lTmp);
}
