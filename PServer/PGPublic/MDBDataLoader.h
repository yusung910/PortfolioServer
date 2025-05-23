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

    //_Read함수들을 실행하는 함수
    bool LoadMDBDatas(MDBDatas& _datas);

private:
    //MasterDB에 Table이 추가 될때마다 함수 생성.
    bool _ReadAbilityComponents(MDBDatas& _datas);
    bool _ReadAbilityModule(MDBDatas& _datas);
    bool _ReadPilgrimLevelStatus(MDBDatas& _datas);

    bool _ReadMapInfo(MDBDatas& _datas);
};

