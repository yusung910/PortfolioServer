#include "stdafx.hxx"
#include "DBWorker.h"
#include "DBWorkerPO.hxx"

DBWorker::DBWorker()
{
    m_pDBWorkerPO = new DBWorkerPO();
}

DBWorker::~DBWorker()
{
    SafeDelete(m_pDBWorkerPO);
}

void DBWorker::SetDBConfig(const std::string& _provider, const std::string& _userID, const std::string& _password, const std::string& _database, const std::string& _host, const std::string& _port)
{
    if (nullptr != m_pDBWorkerPO)
        m_pDBWorkerPO->SetDBConfig(_provider, _userID, _password, _database, _host, _port);
}

bool DBWorker::Init()
{
    if (nullptr == m_pDBWorkerPO)
        return false;
    return m_pDBWorkerPO->Init();
}

CSession* DBWorker::GetSession()
{
    if (nullptr == m_pDBWorkerPO)
        return nullptr;
    return m_pDBWorkerPO->GetSession();
}

bool DBWorker::IsConnected()
{
    return m_pDBWorkerPO->IsConnected();
}

