#pragma once
#include "SCoreAPI.h"

#include <string>
#include <atomic>

// Poco Include ����
#ifndef POCO_STATIC
#	define POCO_STATIC 1
#endif

#pragma warning (push)
#pragma warning (disable: 4244 4245 4100 4127)
#pragma warning (disable: 4634 4635)	// XML �ּ� ����
#pragma warning (disable: 4389)			// Visual Studio 2022 (vc 143) , signed, unsigned ����ġ
#pragma warning (disable: 26439 26498 26451 26444 26451 26812)	// VC++ Code Guidelines warning. POCO ���̺귯���� ����...

#include <Poco/DateTime.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/ODBC/ODBCException.h>
#include <Poco/Data/ODBC/ODBCStatementImpl.h>


#pragma warning (pop)

#pragma comment (lib, "iphlpapi.lib")	// GetAdaptersInfo()

using namespace Poco::Data::Keywords;		// in() out()

class DBWorkerPO;

class SCoreAPI DBWorker
{
private:
    DBWorkerPO* m_pDBWorkerPO;

public:
    DBWorker();
    ~DBWorker();

    void SetDBConfig(const std::string& _driver
        , const std::string& _userID
        , const std::string& _password
        , const std::string& _database
        , const std::string& _host
        , const std::string& _port);

    bool Init();

    Poco::Data::Session* GetSession();

    bool IsConnected() const;
};

