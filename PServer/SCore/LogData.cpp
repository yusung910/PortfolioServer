#include "stdafx.hxx"
#include "LogData.hxx"
#include "StringUtil.h"
#include <ctime>

LogData::LogData()
{
    m_sLogMsg.reserve(MAX_LOG_STRING_SIZE);
}

LogData::LogData(const ELogLevel& _lvl, const std::string& _msg, const bool& _showConsole, const bool& _writeFile)
{
    SetData(_lvl, _msg, _showConsole, _writeFile);
}

LogData::LogData(const ELogLevel& _lvl, const std::wstring& _msg, const bool& _showConsole, const bool& _writeFile)
{
    SetData(_lvl, _msg, _showConsole, _writeFile);
}

void LogData::SetData(const ELogLevel& _lvl, const std::string& _msg, const bool& _showConsole, const bool& _writeFile)
{
    SetData(_lvl, StringUtil::ToWideChar(_msg), _showConsole, _writeFile);
}

void LogData::SetData(const ELogLevel& _lvl, const std::wstring& _msg, const bool& _showConsole, const bool& _writeFile)
{
    m_eLogLvl = _lvl;
    m_sLogMsg.assign(_msg.begin(), _msg.end());
    m_oLogTime = std::chrono::system_clock::now();
    m_bShowConsole = _showConsole;
    m_bWriteFile = _writeFile;

}

std::wstring LogData::ToString()
{
    std::tm localTimeData;
    time_t localLogTime = std::chrono::system_clock::to_time_t(m_oLogTime);
    if (0 != localtime_s(&localTimeData, &localLogTime))
        return L"Timne Error!!";

    wchar_t localStrTmp[50] = { 0, };

    //timeptr 구조의 시간 및 날짜 스펙을 와이드 문자 스트링으로 변환
    size_t localOffset = wcsftime(localStrTmp, _countof(localStrTmp), L"[%Y-%m-%d %H:%M:%S]", &localTimeData);

    //
    int localMS = (int)(std::chrono::duration_cast<std::chrono::milliseconds>(m_oLogTime.time_since_epoch()).count() & 1000);

    swprintf_s(localStrTmp + localOffset, 50 - localOffset, L".%03d]", localMS);

    std::wstring localStrRet(localStrTmp);
    localStrRet.append(L" ");
    localStrRet.append(m_sLogMsg);
    
    return localStrRet;
}


