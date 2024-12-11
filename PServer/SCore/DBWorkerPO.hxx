/*!
 *  @file DBWorkerPO.hxx
 *  @author ShootingStar
 *  @date 2024-11-30
 *  @project SCore
 *
 *  DB �۾��� ���� Ŭ����
 */
#pragma once
#pragma warning(disable : 4996)
#include <string>
#include <atomic>
#include <atldbcli.h>  
#include "DBSession.hxx"

constexpr size_t PROPERTY_MAX_FIELD_SIZE = 8192;
constexpr size_t DEFAULT_DATABASE_TIMEOUT = 5;		// ������ Ȯ�� �ʿ�
constexpr int MAX_CONNECT_TRY_COUNT = 20;			// �ִ� ������ �õ� Ƚ��

typedef CCommand<CDynamicParameterAccessor, CRowset, CMultipleResults>    CxParamCmd;

class DBWorkerPO
{
private:
	static std::atomic_bool m_bInitialized;

	CDataSource  m_ds;
    DBSession    m_oSession;
	std::string  m_sConnection;
	std::string  m_sDBName;
	std::string  m_sProvider;
	HRESULT      m_oHr = S_FALSE;
	int m_nReconnectFailCount = 0;

	CxParamCmd m_oCmd;

public:
	DBWorkerPO() = default;
	~DBWorkerPO();

	void SetDBConfig(const std::string& _provider
		, const std::string& _userID
		, const std::string& _password
		, const std::string& _database
		, const std::string& _host
		, const std::string& _port);

    DBSession GetSession();

	bool IsConnected() const;

private:
	bool _ConnectDB();
	void _CheckDBConnection();
};

