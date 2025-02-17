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

    //_Read함수들을 실행하는 함수
    bool LoadMDBDatas(MDBDatas& _datas);

private:
    //MasterDB에 Table이 추가 될때마다 함수 생성.
    bool _ReadHeroStat(MDBDatas& _datas);

};

