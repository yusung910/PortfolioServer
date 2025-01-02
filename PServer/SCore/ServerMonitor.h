#pragma once
#include <stdint.h>
#include <string>

#include <RefSingleton.h>

// CPU 사용률 구하는 헤더
//PDH :: Performancer Data Helper
#include <Pdh.h>	//PerformanceCounter Query
#pragma comment(lib, "pdh.lib")

class SCoreAPI ServerMonitor : public RefSingleton <ServerMonitor>
{
private:
	int m_nCPUNums = 0;
	//https://m.blog.naver.com/lhk0523/140153391828
	//하드웨어 정보를 가져오는데 사용되는 구조체
	ULARGE_INTEGER m_oLastCPU = {};
	ULARGE_INTEGER m_oLastSystemCPU = {};
	ULARGE_INTEGER m_oLastUserCPU = {};

	//handle
	PDH_HQUERY m_oCPUQuery;
	PDH_HCOUNTER m_oCPUCounter;

public:
	ServerMonitor();
	virtual ~ServerMonitor();

	int64_t GetCurrentMemory() const;
	std::string GetCurrentMemoryString(const int64_t& _mem) const;
	std::string GetCurrentMemoryString() const;


	double GetCurrentCPUUsage();
	std::string GetCurrentCPUUsageString(const double& _usage);
	std::string GetCurrentCPUUsageString();

	// 머신 전체 CPU
	double GetMachineCPUUsage() const;


	// 머신 전체 메모리 사용률
	double GetMachineMemoryUsage() const;
	double GetMachineMemoryUsage(int64_t& _usage, int64_t& _total) const;	// 단위 : byte

	// 머신 디스크 사용률
	double GetMachineDiskUsage(const std::wstring& _strDrive = L"C:") const;
	double GetMachineDiskUsage(const std::wstring& _strDrive, int64_t& _usage, int64_t& _total) const;	// 단위 : byte

};

