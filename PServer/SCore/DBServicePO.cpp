#include "stdafx.hxx"
#include "DBServicePO.hxx"

void DBServicePO::SetDBConfig(const std::string& _userID, const std::string& _password, const std::string& _database, const std::string& _host, const std::string& _port)
{
    if (true == m_oWorker.IsConnected())
    {
        VIEW_WARNING("Database is Already Connected");
        return;
    }

    m_oWorker.SetDBConfig("SQL Server", _userID, _password, _database, _host, _port);

    if (false == m_oWorker.Init())
    {
        VIEW_WRITE_ERROR("Database Initialize Failed !! Server: %s, DB: %s", _host.c_str(), _database.c_str());
    }
    else
    {
        VIEW_WRITE_ERROR("Database(Server: %s, DB: %s)Initialize", _host.c_str(), _database.c_str());
    }
}


Poco::Data::Session* DBServicePO::GetSession()
{
    return m_oWorker.GetSession();
}

void DBServicePO::Exit()
{
    m_ShouldExit = true;
}

bool DBServicePO::Push(InnerPacket::SharedPtr _data)
{
    if (nullptr == _data.get())
        return false;

    m_oPacketQueue.Push(_data);
    return true;
}

const int& DBServicePO::GetServerID() const noexcept
{
    return m_nServerID;
}

void DBServicePO::SetServerID(const int& _serverID) noexcept
{
    m_nServerID = _serverID;
}

bool DBServicePO::CreateThread()
{
    if (INVALID_HANDLE_VALUE != m_hThread
        && NULL != m_hThread)
        return false;

    m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, this, 0, nullptr);
    return m_hThread != INVALID_HANDLE_VALUE;
}

size_t DBServicePO::GetWorkQueueCount()
{
    return m_oPacketQueue.Count();
}

unsigned int DBServicePO::Run()
{
    while (false == m_ShouldExit)
    {
        //timer 실행
        UpdateTimer();

        m_oPacketQueue.Swap(m_oWorkList);

        if (true == m_oWorkList.empty())
        {
            Sleep(1);
            continue;
        }

        for (auto& data : m_oWorkList)
        {
            if (auto it = m_umHandlers.find(data->m_nProtocol); it != m_umHandlers.end())
            {
                if (nullptr != it->second)
                    it->second(data);
            }
        }
        m_oWorkList.clear();
    }

    return 0;
}

void DBServicePO::RegisterHandler(const int& _protocolID, const std::function<bool(InnerPacket::SharedPtr)>& _pFunc)
{
    m_umHandlers.insert_or_assign(_protocolID, _pFunc);
}

unsigned int __stdcall DBServicePO::ThreadFunc(void* _pSelf)
{
    if (nullptr == _pSelf)
        return 0;
    return static_cast<DBServicePO*>(_pSelf)->Run();
}
