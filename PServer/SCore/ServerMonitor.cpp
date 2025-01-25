#include "stdafx.hxx"
#include "ServerMonitor.h"

#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

#include <ctime>

ServerMonitor::ServerMonitor()
{
	PdhOpenQuery(NULL, NULL, &m_oCPUQuery);
	PdhAddCounter(m_oCPUQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &m_oCPUCounter);

	PdhCollectQueryData(m_oCPUQuery);
}

ServerMonitor::~ServerMonitor()
{
	PdhRemoveCounter(m_oCPUCounter);
	PdhCloseQuery(m_oCPUQuery);
}

int64_t ServerMonitor::GetCurrentMemory() const
{
	int64_t lRet = -1;

	//지금 실행중인 프로세스의 ID
	DWORD lDwProcessID = GetCurrentProcessId();
	PROCESS_MEMORY_COUNTERS lPMC;

	//OpenProcess(): 기존 프로세스의 Handle을 가져온다
	//https://learn.microsoft.com/ko-kr/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess
	HANDLE lHandleProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lDwProcessID);
	if (NULL == lHandleProcess)
		return lRet;
	
	if (TRUE == GetProcessMemoryInfo(lHandleProcess, &lPMC, sizeof(lPMC)))
		lRet = (int64_t)lPMC.WorkingSetSize;

	CloseHandle(lHandleProcess);
	return lRet;
}

std::string ServerMonitor::GetCurrentMemoryString(const int64_t& _mem) const
{
	int64_t lSize = _mem;
	double lRet = 0;
	char lTmp[20] = { 0, };

	if (lSize < 1024)
		return std::string("Mem " + std::to_string(lSize) + " Bytes");

	lSize >>= 10;

	if (lSize < 1024)
	{
		lRet = _mem / 1024.0;
		sprintf_s(lTmp, sizeof(lTmp), "Mem %.2f KB", lRet);
		return std::string(lTmp);
	}

	lSize >>= 10;

	if (lSize < 1024)
	{
		lRet = _mem / 1024.0 / 1024.0;
		sprintf_s(lTmp, sizeof(lTmp), "Mem %.2f MB", lRet);
		return std::string(lTmp);
	}

	lRet = _mem / 1024.0 / 1024.0 / 1024.0;
	sprintf_s(lTmp, sizeof(lTmp), "Mem %.2f GB", lRet);
	return std::string(lTmp);
}

std::string ServerMonitor::GetCurrentMemoryString() const
{
	return GetCurrentMemoryString(GetCurrentMemory());
}

double ServerMonitor::GetCurrentCPUUsage()
{
	if (m_nCPUNums == 0)
	{
		SYSTEM_INFO lInfo;
		GetSystemInfo(&lInfo);
		m_nCPUNums = lInfo.dwNumberOfProcessors;
	}

	HANDLE lHProcess = GetCurrentProcess();

	FILETIME lfTime, lfSystem, lfUser;
	ULARGE_INTEGER lNow, lSys, lUser;

	double lPer;

	GetSystemTimeAsFileTime(&lfTime);
	memcpy(&lNow, &lfTime, sizeof(FILETIME));

	GetProcessTimes(lHProcess, &lfTime, &lfTime, &lfSystem, &lfUser);
	memcpy(&lSys, &lfSystem, sizeof(FILETIME));
	memcpy(&lUser, &lfUser, sizeof(FILETIME));

	lPer = (double)((lSys.QuadPart - m_oLastSystemCPU.QuadPart) + (lUser.QuadPart - m_oLastUserCPU.QuadPart));

	lPer /= (double)(lNow.QuadPart - m_oLastCPU.QuadPart);
	lPer /= (double)m_nCPUNums;

	m_oLastCPU = lNow;
	m_oLastUserCPU = lUser;
	m_oLastSystemCPU = lSys;

	CloseHandle(lHProcess);

	return lPer * 100.0;
}

std::string ServerMonitor::GetCurrentCPUUsageString(const double& _usage)
{
	char lTmp[20] = { 0, };
	sprintf_s(lTmp, sizeof(lTmp), "CPU %.2f", _usage);
	return std::string(lTmp);
}

std::string ServerMonitor::GetCurrentCPUUsageString()
{
	return GetCurrentCPUUsageString(GetCurrentCPUUsage());
}

double ServerMonitor::GetMachineCPUUsage() const
{
	double lUsage = 0.0;

	PDH_FMT_COUNTERVALUE lCounterVal;
	PdhCollectQueryData(m_oCPUQuery);

	if (ERROR_SUCCESS == PdhGetFormattedCounterValue(m_oCPUCounter, PDH_FMT_DOUBLE, NULL, &lCounterVal))
		lUsage = lCounterVal.doubleValue;

	return lUsage;
}

double ServerMonitor::GetMachineMemoryUsage() const
{
	int64_t lUse = 0;
	int64_t lTotal = 0;
	return GetMachineMemoryUsage(lUse, lTotal);
}

double ServerMonitor::GetMachineMemoryUsage(int64_t& _usage, int64_t& _total) const
{
	MEMORYSTATUSEX lMemStat;
	memset(&lMemStat, 0x00, sizeof(MEMORYSTATUSEX));
	lMemStat.dwLength = sizeof(lMemStat);

	//dwLength은 초기 입력 값(구조체 크기)
	//	dwMemoryLoad: 메모리 사용량(%)
	//	ullTotalPhys : 실제 총 물리 메모리 크기
	//	ullAvailPhys : 물리 메모리 중에 사용가능한 메모리
	//	ullTotalPageFile : 총 페이지 파일 크기
	//	ullAvailPageFile : 사용 가능한 페이지 파일 크기
	//	ullTotalVirtual : 총 가상 메모리 크기
	//	ullAvailExtendedVirtual : 확장 메모리 크기.

	if (TRUE == GlobalMemoryStatusEx(&lMemStat))
	{
		_usage = lMemStat.ullTotalPhys - lMemStat.ullAvailPhys;
		_total = lMemStat.ullTotalPhys;

		if (_total <= 0)
			return 0.0;

		return (1.0 * _usage) / (1.0 * _total) * 100.0;
	}

	return 0.0;
}

double ServerMonitor::GetMachineDiskUsage(const std::wstring& _strDrive) const
{
	int64_t lUse = 0;
	int64_t lTotal = 0;
	return GetMachineDiskUsage(_strDrive, lUse, lTotal);
}

double ServerMonitor::GetMachineDiskUsage(const std::wstring& _strDrive, int64_t& _usage, int64_t& _total) const
{
	ULARGE_INTEGER lFree;
	ULARGE_INTEGER lTotal;
	ULARGE_INTEGER lTotalOfFree;

	if (FALSE == GetDiskFreeSpaceEx(_strDrive.c_str(), &lFree, &lTotal, &lTotalOfFree))
		return 0.0;


	_usage = lTotal.QuadPart - lFree.QuadPart;
	_total = lTotal.QuadPart;

	if (_total <= 0.0)
		return 0.0;

	return (1.0 * _usage) / (1.0 * _total) * 100.0;
}
