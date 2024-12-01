#include "stdafx.hxx"
#include "DBWorkerPO.hxx"
#pragma warning(disable : 4996)

std::atomic_bool DBWorkerPO::m_bInitialized = true;

DBWorkerPO::~DBWorkerPO()
{
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
    sprintf_s(localTmpConnection, std::size(localTmpConnection), "PROVIDER=%s;SERVER=%s,%s;UID=%s;PWD=%s;DATABASE=%s", _provider, _host, _port, _userID, _password, _database);

    m_sConnection.assign(localTmpConnection);
    m_sDBName.assign(_database);
    m_sProvider.assign(_provider);
}

bool DBWorkerPO::Init()
{
    try
    {
        //COM 초기화  
        m_ohr = CoInitialize(0);
        if (FAILED(m_ohr))
        {
            m_bInitialized = false;
            VIEW_WRITE_ERROR("DBWorker::Init() - CoInitialize(0) Fail!");
            return false;
        }

        //데이터베이스 연결  
        CDBPropSet dbinit(DBPROPSET_DBINIT);
        dbinit.AddProperty(DBPROP_INIT_PROMPT, (SHORT)4);
        dbinit.AddProperty(DBPROP_INIT_PROVIDERSTRING, m_sConnection.c_str());
        dbinit.AddProperty(DBPROP_INIT_LCID, (LONG)1043); //->Locale identifier  
        m_ohr = m_ds.Open(_T("SQLOLEDB"), &dbinit);

        if (FAILED(m_ohr))
        {
            m_bInitialized = false;
            VIEW_WRITE_ERROR("DBWorker::Init() - m_ds.Open() Fail!");
            return false;
        }

        _ConnectDB();
    }
    catch (...)
    {

    }
    return true;
}

CSession DBWorkerPO::GetSession()
{
    _CheckDBConnection();
    return m_oSession;
}

bool DBWorkerPO::_ConnectDB()
{
    //세션을 시작합니다.  
    m_ohr = m_oSession.Open(m_ds);
    if (FAILED(m_ohr))
    {
        VIEW_WRITE_ERROR("DBWorker::_ConnectDB() - m_ds.Open() Fail!");
        m_ds.Close();
        return false;
    }
    return true;
}

bool DBWorkerPO::_CheckDBConnection()
{
    return false;
}
