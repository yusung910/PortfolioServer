/*!
 *  @file DBWorkerPO.hxx
 *  @author ShootingStar
 *  @date 2024-11-30
 *  @project SCore
 *
 *  DB 작업을 위한 클래스
 *  OLEDB를 이용해서 MSSQL과 연결할 클래스
 *  https://fiadot.tistory.com/477
 */
#pragma once
#pragma warning(disable : 4996)
#include <string>
#include <atomic>
#include <atldbcli.h>  


constexpr size_t PROPERTY_MAX_FIELD_SIZE = 8192;
constexpr size_t DEFAULT_DATABASE_TIMEOUT = 5;		// 단위는 확인 필요
constexpr int MAX_CONNECT_TRY_COUNT = 20;			// 최대 재접속 시도 횟수


typedef CCommand<CDynamicParameterAccessor, CRowset, CMultipleResults>    CxParamCmd;

class DBWorkerPO
{
private:
	static std::atomic_bool m_bInitialized;

	CDataSource  m_ds;
	CSession     m_oSession;
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

	bool Init();

	//때에따라 삭제 예정
	CSession GetSession();

	CxParamCmd GetQueryCMD();

	bool IsConnected() const;
	//bool operator << (const TCHAR* _query)
	//{
	//	void* localpDummy;

	//	//Create
	//	m_oHr = m_oCmd.Create(m_oSession, _query);
	//	if (FAILED(m_oHr))
	//	{
	//		AtlTraceErrorRecords(m_oHr);
	//		VIEW_WRITE_ERROR("DBWorker::SetQuery() - Create() Fail!!(%d)", m_oHr);
	//		return false;
	//	}

	//	m_oHr = m_oCmd.Prepare();
	//	if (FAILED(m_oHr))
	//	{
	//		AtlTraceErrorRecords(m_oHr);
	//		VIEW_WRITE_ERROR("DBWorker::SetQuery() - Prepare() Fail!!(%d)", m_oHr);
	//		return false;
	//	}

	//	//쿼리 실행에 필요한 파라미터를 세팅
	//	m_oHr = m_oCmd.BindParameters(&m_oCmd.m_hParameterAccessor, m_oCmd.m_spCommand, &localpDummy);

	//	return true;
	//}



	void ExecuteQuery();

	template<typename T>
	void SetQueryResult(T& _data)
	{
		m_oHR = m_oCmd.MoveFirst();
		while (m_oHR == S_OK)
		{

			m_oHR = m_oCmd.MoveNext();
		}
	}
private:
	bool _ConnectDB();
	void _CheckDBConnection();
};

