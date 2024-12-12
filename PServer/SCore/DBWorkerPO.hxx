/*!
 *  @file DBWorkerPO.hxx
 *  @author ShootingStar
 *  @date 2024-11-30
 *  @project SCore
 *
 *  DB 작업을 위한 클래스
 */
#pragma once
#pragma warning(disable : 4996)
#include <string>
#include <atomic>
 // Poco Include 관련
#ifndef POCO_STATIC
#	define POCO_STATIC 1
#endif

#pragma warning (push)
#pragma warning (disable: 4244 4245 4100 4127)
#pragma warning (disable: 4634 4635)	// XML 주석 관련
#pragma warning (disable: 4389)			// Visual Studio 2022 (vc 143) , signed, unsigned 불일치
#pragma warning (disable: 26439 26498 26451 26444 26451 26812)	// VC++ Code Guidelines warning. POCO 라이브러리가 문제...

#include <Poco/DateTime.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/ODBC/ODBCException.h>
#include <Poco/Data/ODBC/ODBCStatementImpl.h>

#pragma warning (pop)

#pragma comment (lib, "iphlpapi.lib")	// GetAdaptersInfo()

namespace Poco::Data
{
	class Session;
}

using namespace Poco::Data::Keywords;		// in() out()


constexpr size_t PROPERTY_MAX_FIELD_SIZE = 8192;
constexpr size_t DEFAULT_DATABASE_TIMEOUT = 5;		// 단위는 확인 필요
constexpr int MAX_CONNECT_TRY_COUNT = 20;			// 최대 재접속 시도 횟수

class DBWorkerPO
{
private:
	static std::atomic_bool m_bInitialized;

	Poco::Data::Session* m_pSession = nullptr;

	std::string  m_sConnection;
	std::string  m_sODBCDriver;
	std::string  m_sDBName;
	int m_nReconnectFailCount = 0;

public:
	DBWorkerPO() = default;
	~DBWorkerPO();

	void SetDBConfig(const std::string& _driver
		, const std::string& _userID
		, const std::string& _password
		, const std::string& _database
		, const std::string& _host
		, const std::string& _port);

	bool Init();

    Poco::Data::Session* GetSession();

	bool IsConnected() const;

private:
	bool _ConnectDB();
	void _CheckDBConnection();
};

