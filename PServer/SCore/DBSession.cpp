#include "stdafx.hxx"
#include "DBSession.hxx"
#include "DBStatements.h"


DBSession::DBSession()
{

}

DBSession::~DBSession()
{
    m_oSession.Close();
}

void DBSession::Init(std::string _connectionStr)
{
    try
    {
        //COM 초기화  
        m_sConnection.assign(_connectionStr);
        //m_oHr = CoInitialize(0);
        _ConnectDB();
    }
    catch (...)
    {
        VIEW_WRITE_ERROR("DBWorker Exception !!");
    }
}

bool DBSession::IsConnected() const
{
    return m_oStatements->IsConnected();
}

HRESULT DBSession::ExecuteQuery()
{
    // Open the command.
    HRESULT localHR = S_FALSE;
    localHR = m_oCmd.Open(NULL, NULL, 0);
    m_oCmd.Bind();

    return m_oCmd.MoveFirst();
}

void DBSession::_ConnectDB()
{
    //데이터베이스 연결  
    HRESULT localHR = S_FALSE;
    CDBPropSet dbinit(DBPROPSET_DBINIT);
    dbinit.AddProperty(DBPROP_INIT_PROMPT, (SHORT)4);
    dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, m_sConnection.c_str());
    dbinit.AddProperty(DBPROP_INIT_LCID, (LONG)1043); //->Locale identifier  
    localHR = m_ds.Open(_T("SQLOLEDB"), &dbinit);

    //세션을 시작합니다.  
    localHR = m_oSession.Open(m_ds);
    if (!FAILED(localHR))
    {
        m_oStatements = new DBStatements(localHR);
    }
}
