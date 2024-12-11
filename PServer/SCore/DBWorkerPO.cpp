#include "stdafx.hxx"
#include "DBWorkerPO.hxx"

std::atomic_bool DBWorkerPO::m_bInitialized = true;

DBWorkerPO::~DBWorkerPO()
{
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

    DBSession session;
    session << "exec TEST, ?, ?, OUT";
}

DBSession DBWorkerPO::GetSession()
{
    _CheckDBConnection();
    return m_oSession;
}


bool DBWorkerPO::IsConnected() const
{
    return !FAILED(m_oHr);
}

bool DBWorkerPO::_ConnectDB()
{
}

void DBWorkerPO::_CheckDBConnection()
{

}
