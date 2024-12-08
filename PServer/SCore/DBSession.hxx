#pragma once
#include <atldbcli.h>  
#include "ObjectQueue.h"


typedef CCommand<CDynamicParameterAccessor, CRowset, CMultipleResults>    CxParamCmd;

class DBSession
{
private:
	std::string		m_sConnection = "";
	HRESULT			m_oHr = S_FALSE;
	CDataSource		m_ds;
	CSession		m_oSession;
	CxParamCmd		m_oCmd;

public:
	DBSession();
	~DBSession();

	void Init(std::string _connectionStr);
	bool IsConnected() const;


	inline void operator << (const TCHAR* _query)
	{
		void* localpDummy;

		//Create
		m_oHr = m_oCmd.Create(m_oSession, _query);
		if (FAILED(m_oHr))
		{
			AtlTraceErrorRecords(m_oHr);
			VIEW_WRITE_ERROR("DBWorker::SetQuery() - Create() Fail!!(%d)", m_oHr);
		}

		m_oHr = m_oCmd.Prepare();
		if (FAILED(m_oHr))
		{
			AtlTraceErrorRecords(m_oHr);
			VIEW_WRITE_ERROR("DBWorker::SetQuery() - Prepare() Fail!!(%d)", m_oHr);
		}

		//쿼리 실행에 필요한 파라미터를 세팅
		m_oHr = m_oCmd.BindParameters(&m_oCmd.m_hParameterAccessor, m_oCmd.m_spCommand, &localpDummy);
	}

private:
	void _ConnectDB();
};

