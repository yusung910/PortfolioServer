#include "stdafx.hxx"
#include "DBWorkerPO.hxx"

std::atomic_bool DBWorkerPO::m_bInitialized = false;

DBWorkerPO::~DBWorkerPO()
{
    SafeDelete(m_pSession);
}

void DBWorkerPO::SetDBConfig(const std::string& _driver
    , const std::string& _userID
    , const std::string& _password
    , const std::string& _database
    , const std::string& _host
    , const std::string& _port)
{
    char lTmpConnection[512] = { 0, };

    //driver = SQLOLEDB.1
    sprintf_s(lTmpConnection, std::size(lTmpConnection), "DRIVER=%s;UID=%s;PWD=%s;DATABASE=%s;SERVER=%s,%s;", _driver.c_str(), _userID.c_str(), _password.c_str(), _database.c_str(),_host.c_str(), _port.c_str());

    m_sConnection.assign(lTmpConnection);
    m_sODBCDriver.assign(_driver);
    m_sDBName.assign(_database);
}

bool DBWorkerPO::Init()
{
    if (false == m_bInitialized)
    {
        m_bInitialized = true;
        Poco::Data::ODBC::Connector::registerConnector();
    }
    Poco::Data::ODBC::Utility::DriverMap lDrivers;
    Poco::Data::ODBC::Utility::drivers(lDrivers);

    for (auto& driver : lDrivers)
    {
        if (((driver.first).find(m_sODBCDriver)) != std::string::npos)
        {
            return _ConnectDB();
        }
    }

    return false;
}

Poco::Data::Session* DBWorkerPO::GetSession()
{
    _CheckDBConnection();
    return m_pSession;
}


bool DBWorkerPO::IsConnected() const
{
    if (nullptr != m_pSession)
        return m_pSession->isConnected();
    return false;
}

bool DBWorkerPO::_ConnectDB()
{
    try
    {
        Poco::Data::Session* lSession = new Poco::Data::Session(Poco::Data::ODBC::Connector::KEY, m_sConnection, DEFAULT_DATABASE_TIMEOUT);

        lSession->impl()->setProperty("maxFieldSize", Poco::Any(PROPERTY_MAX_FIELD_SIZE));

        std::swap(m_pSession, lSession);

        SafeDelete(lSession);

        VIEW_INFO(L"Connecting to %s", StringUtil::ToWideChar(m_sDBName).c_str());
    }
    catch (Poco::NotFoundException& ex)
    {
        VIEW_WRITE_ERROR(L"NotFoundException::%s", StringUtil::UTF8_WSTR(ex.displayText()).c_str());
    }
    catch (Poco::Data::ConnectionFailedException& ex)
    {
        VIEW_WRITE_ERROR(L"[DB: %s] Connect Failed Cause \n%s", StringUtil::ToWideChar(m_sDBName).c_str(), StringUtil::UTF8_WSTR(ex.displayText()).c_str());
        m_nReconnectFailCount++;
        return false;
    }
    m_nReconnectFailCount = 0;
    return true;
}

void DBWorkerPO::_CheckDBConnection()
{
    if (true == IsConnected())
        return;

    VIEW_WRITE_ERROR(L"[DB: %s] DB Disconnected... Try to Reconnect!", StringUtil::ToWideChar(m_sDBName).c_str());

    while (false == _ConnectDB())
    {
        if (m_nReconnectFailCount >= MAX_CONNECT_TRY_COUNT)
        {
            VIEW_WRITE_ERROR(L"[DB: %s] Connect Fail Try Count is Over", StringUtil::ToWideChar(m_sDBName).c_str());
            return;
        }
    }
}
