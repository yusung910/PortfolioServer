#pragma once
#include <string>
#include "MDBDatas.h"
#include <DBWorker.h>
class MDBDataLoader
{
private:
    DBWorker m_oDBWorker;
public:
    bool SetConfig(const std::string& _userID
        , const std::string& _password
        , const std::string& _database
        , const std::string& _host
        , const std::string& _port);

    //_Read�Լ����� �����ϴ� �Լ�
    bool LoadMDBDatas(MDBDatas& _datas);

private:
    //MasterDB�� Table�� �߰� �ɶ����� �Լ� ����.
    bool _ReadHeroStat(MDBDatas& _datas);

};

