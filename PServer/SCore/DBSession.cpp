#include "stdafx.hxx"
#include "DBSession.hxx"

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
        m_oHr = CoInitialize(0);
        _ConnectDB();
    }
    catch (...)
    {
        VIEW_WRITE_ERROR("DBWorker Exception !!");
    }
}

bool DBSession::IsConnected() const
{
    return !FAILED(m_oHr);
}

HRESULT DBSession::SetQuery(const TCHAR* _query)
{
    void* localpDummy;

    //Create
    m_oHr = m_oCmd.Create(m_oSession, _query);
    //if (FAILED(m_oHr))
    //{
    //    AtlTraceErrorRecords(m_oHr);
    //    VIEW_WRITE_ERROR("DBWorker::SetQuery() - Create() Fail!!(%d)", m_oHr);
    //}

    m_oHr = m_oCmd.Prepare();
    //if (FAILED(m_oHr))
    //{
    //    AtlTraceErrorRecords(m_oHr);
    //    VIEW_WRITE_ERROR("DBWorker::SetQuery() - Prepare() Fail!!(%d)", m_oHr);
    //}

    //쿼리 실행에 필요한 파라미터를 세팅
    m_oHr = m_oCmd.BindParameters(&m_oCmd.m_hParameterAccessor, m_oCmd.m_spCommand, &localpDummy);

    return m_oHr;
}

void DBSession::_ConnectDB()
{
    //데이터베이스 연결  
    CDBPropSet dbinit(DBPROPSET_DBINIT);
    dbinit.AddProperty(DBPROP_INIT_PROMPT, (SHORT)4);
    dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, m_sConnection.c_str());
    dbinit.AddProperty(DBPROP_INIT_LCID, (LONG)1043); //->Locale identifier  
    m_oHr = m_ds.Open(_T("SQLOLEDB"), &dbinit);

    //세션을 시작합니다.  
    m_oHr = m_oSession.Open(m_ds);
}
