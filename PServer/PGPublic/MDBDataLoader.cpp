#include "PGPPrivate.h"
#include "MDBDataLoader.h"
#include "PFunc.h"
#include "PocoTimeUtil.h"
#include <TimeUtil.h>

using namespace Poco::Data::ODBC;

bool MDBDataLoader::SetConfig(const std::string& _userID, const std::string& _password, const std::string& _database, const std::string& _host, const std::string& _port)
{
    m_oDBWorker.SetDBConfig(
        "SQL Server", _userID, _password, _database, _host, _port
    );

    if (false == m_oDBWorker.Init())
    {
        VIEW_WRITE_ERROR(L"DB Initialize Failed: %s / DB: %s", _host.c_str(), _database.c_str());
        return false;

    }
    
    return true;
}

bool MDBDataLoader::LoadMDBDatas(MDBDatas& _datas)
{
    return true;
}

bool MDBDataLoader::_ReadHeroStat(MDBDatas& _datas)
{
    GetDBSession();

    
    return true;
}
