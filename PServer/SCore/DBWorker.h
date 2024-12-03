#pragma once
#include "SCoreAPI.h"
#include <string>
#include <atomic>
#include <atldbcli.h>  

class DBWorkerPO;

class SCoreAPI DBWorker
{
private:
    DBWorkerPO* m_pDBWorkerPO;

public:
    DBWorker();
    ~DBWorker();

    void SetDBConfig(const std::string& _provider
        , const std::string& _userID
        , const std::string& _password
        , const std::string& _database
        , const std::string& _host
        , const std::string& _port);

    bool Init();

    CSession* GetSession();

    bool IsConnected();
};

