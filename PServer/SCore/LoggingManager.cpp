#include "stdafx.hxx"
#include "LoggingManager.h"
#include "LoggingManagerPO.hxx"
#include "StringUtil.h"

// ���� ������ ����
// �޸��� ���ÿ����� ����Ǵ� �������� ũ�Ⱑ ���� ��ġ �Ѿ ���
// C6262 ��� �޼���(���� X)�� ����Ѵ�. �� ��� �޼����� �ذ��ϱ� ����
// �ش� �����͸� �� ������ �����ؾ� �Ѵ�
// �� ������ �����ϱ� ���� �����Ҵ��� �̿��Ѵ�.
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
    //unique_ptr, shared_ptr(������)�� delete�� �� ���� �� �⺻ Destructor(�Ҹ���)�� ����Ѵ�
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
    //unique_ptr, shared_ptr(������)�� delete�� �� ���� �� �⺻ Destructor(�Ҹ���)�� ����Ѵ�
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
    //unique_ptr, shared_ptr(������)�� delete�� �� ���� �� �⺻ Destructor(�Ҹ���)�� ����Ѵ�
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
    //unique_ptr, shared_ptr(������)�� delete�� �� ���� �� �⺻ Destructor(�Ҹ���)�� ����Ѵ�
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
