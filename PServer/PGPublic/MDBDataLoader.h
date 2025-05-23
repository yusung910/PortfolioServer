#pragma once
#include <string>
#include <DBWorker.h>

#include "MDBDatas.h"
#include "MDBDataManager.h"
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
    bool _ReadAbilityComponents(MDBDatas& _datas);
    bool _ReadAbilityModule(MDBDatas& _datas);
    bool _ReadPilgrimLevelStatus(MDBDatas& _datas);

    bool _ReadMapInfo(MDBDatas& _datas);
};

