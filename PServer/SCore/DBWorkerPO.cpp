#include "stdafx.hxx"
#include "DBWorkerPO.hxx"

std::atomic_bool DBWorkerPO::m_bInitialized = true;

DBWorkerPO::~DBWorkerPO()
{
    m_oSession.Close();
    m_ds.Close();
}

void DBWorkerPO::SetDBConfig(const std::string& _provider
    , const std::string& _userID
    , const std::string& _password
    , const std::string& _database
    , const std::string& _host
    , const std::string& _port)
{
    char localTmpConnection[512] = { 0, };

    //driver = SQLOLEDB.1
    sprintf_s(localTmpConnection, std::size(localTmpConnection), "PROVIDER=%s;SERVER=%s,%s;UID=%s;PWD=%s;DATABASE=%s", _provider.c_str(), _host.c_str(), _port.c_str(), _userID.c_str(), _password.c_str(), _database.c_str());

    m_sConnection.assign(localTmpConnection);
    m_sDBName.assign(_database);
    m_sProvider.assign(_provider);
}

bool DBWorkerPO::Init()
{
    try
    {
        //COM 초기화  
        m_oHr = CoInitialize(0);
        if (FAILED(m_oHr))
        {
            m_bInitialized = false;
            VIEW_WRITE_ERROR("DBWorker::Init() - CoInitialize(0) Fail!");
            return false;
        }

        _ConnectDB();
    }
    catch (...)
    {
        VIEW_WRITE_ERROR("DBWorker Exception !!");
    }
    return true;
}

CSession DBWorkerPO::GetSession()
{
    _CheckDBConnection();
    return m_oSession;
}

bool DBWorkerPO::IsConnected() const
{
    return !FAILED(m_oHr);
}

bool DBWorkerPO::SetQuery(TCHAR* _query)
{
    //Create
    m_oHr = m_oCmd.Create(m_oSession, _query);
    if (FAILED(m_oHr))
    {
        AtlTraceErrorRecords(m_oHr);
        VIEW_WRITE_ERROR("DBWorker::SetQuery() - Create() Fail!!(%d)", m_oHr);
        return false;
    }

    m_oHr = m_oCmd.Prepare();
    if (FAILED(m_oHr))
    {
        AtlTraceErrorRecords(m_oHr);
        VIEW_WRITE_ERROR("DBWorker::SetQuery() - Prepare() Fail!!(%d)", m_oHr);
        return false;
    }

    void* localpDummy;

    //쿼리 실행에 필요한 파라미터를 세팅
    m_oHr = m_oCmd.BindParameters(&m_oCmd.m_hParameterAccessor, m_oCmd.m_spCommand, &localpDummy);

    //va_list localParams;
    //va_start(localParams, _query);
    //_vsnwprintf_s(localpBuffer, MAX_LOG_STRING_SIZE, MAX_LOG_STRING_SIZE - 1, _fmt, localArgs);
    //va_end(localParams);


    return true;
}

bool DBWorkerPO::_ConnectDB()
{
    //데이터베이스 연결  
    CDBPropSet dbinit(DBPROPSET_DBINIT);
    dbinit.AddProperty(DBPROP_INIT_PROMPT, (SHORT)4);
    dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, m_sConnection.c_str());
    dbinit.AddProperty(DBPROP_INIT_LCID, (LONG)1043); //->Locale identifier  
    m_oHr = m_ds.Open(_T("SQLOLEDB"), &dbinit);

    if (FAILED(m_oHr))
    {
        VIEW_WRITE_ERROR("DBWorker::_ConnectDB() - m_ds.Open() Fail!");
        m_nReconnectFailCount++;
        return false;
    }

    //세션을 시작합니다.  
    m_oHr = m_oSession.Open(m_ds);
    if (FAILED(m_oHr))
    {
        VIEW_WRITE_ERROR("DBWorker::_ConnectDB() - m_oSession.Open() Fail!");
        m_ds.Close();
        m_nReconnectFailCount++;
        return false;
    }
    return true;
}

void DBWorkerPO::_CheckDBConnection()
{
    if (true == IsConnected())
        return;

    VIEW_WRITE_ERROR("[DB : %s] Database Disconnected... Try to Reconnecting...", m_sDBName.c_str());

    while (false == _ConnectDB())
    {
        if (m_nReconnectFailCount >= MAX_CONNECT_TRY_COUNT)
        {
            // 이정도 재시도 했는데 전부 실패일 경우 DB 사망 또는 네트워크가 끊겼을 경우
            VIEW_WRITE_ERROR("[DB: %s] Connect Fail Count Over!", m_sDBName.c_str());
            return;
        }
    }

}
