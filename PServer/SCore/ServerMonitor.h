#pragma once
#include <stdint.h>
#include <string>

#include <RefSingleton.h>

// CPU ���� ���ϴ� ���
//PDH :: Performancer Data Helper
#include <Pdh.h>	//PerformanceCounter Query
#pragma comment(lib, "pdh.lib")

class SCoreAPI ServerMonitor : public RefSingleton <ServerMonitor>
{
private:
	int m_nCPUNums = 0;
	//https://m.blog.naver.com/lhk0523/140153391828
	//�ϵ���� ������ �������µ� ���Ǵ� ����ü
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

	// �ӽ� ��ü CPU
	double GetMachineCPUUsage() const;


	// �ӽ� ��ü �޸� ����
	double GetMachineMemoryUsage() const;
	double GetMachineMemoryUsage(int64_t& _usage, int64_t& _total) const;	// ���� : byte

	// �ӽ� ��ũ ����
	double GetMachineDiskUsage(const std::wstring& _strDrive = L"C:") const;
	double GetMachineDiskUsage(const std::wstring& _strDrive, int64_t& _usage, int64_t& _total) const;	// ���� : byte

};

