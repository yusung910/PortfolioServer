#include "stdafx.hxx"
#include "DBService.h"
#include "DBServicePO.hxx"

DBService::DBService()
{
    m_pDBServicePO = new DBServicePO();
}

DBService::~DBService()
{
    Exit();
    SafeDelete(m_pDBServicePO);
}

void DBService::SetDBConfig(const std::string& _userID, const std::string _password, const std::string& _database, const std::string& _host, const std::string& _port)
{
    if (nullptr != m_pDBServicePO)
        m_pDBServicePO->SetDBConfig(_userID, _password, _database, _host, _port);
}

Poco::Data::Session* DBService::GetSession()
{
    if (nullptr == m_pDBServicePO)
        return nullptr;
    return m_pDBServicePO->GetSession();
}

void DBService::Exit()
{
    if (nullptr != m_pDBServicePO)
        m_pDBServicePO->Exit();
}

bool DBService::Push(InnerPacket::SharedPtr _data)
{
    if (nullptr == m_pDBServicePO)
        return false;
    return m_pDBServicePO->Push(_data);
}

int DBService::GetServerID() const noexcept
{
    if (nullptr == m_pDBServicePO)
        return 0;
    return m_pDBServicePO->GetServerID();
}

void DBService::SetServerID(const int& _serverID) noexcept
{
    if (nullptr != m_pDBServicePO)
        m_pDBServicePO->SetServerID(_serverID);
}

bool DBService::CreateThread()
{
    if (nullptr == m_pDBServicePO)
        return false;
    return m_pDBServicePO->CreateThread();
}

size_t DBService::GetWorkQueueCount()
{
    if (nullptr == m_pDBServicePO)
        return size_t(0);
    return m_pDBServicePO->GetWorkQueueCount();
}

unsigned int DBService::Run()
{
    if (nullptr == m_pDBServicePO)
        return 0;
    return m_pDBServicePO->Run();
}


void DBService::_RegisterHandler(const int& _protocolID, const std::function<bool(InnerPacket::SharedPtr)>& _pFunc)
{
    if (nullptr != m_pDBServicePO)
        m_pDBServicePO->RegisterHandler(_protocolID, _pFunc);
}