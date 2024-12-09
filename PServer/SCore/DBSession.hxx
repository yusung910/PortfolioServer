#pragma once
#include <atldbcli.h>  
#include "Object.h"


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

    HRESULT SetQuery(const TCHAR* _query);


private:
	void _ConnectDB();
};

