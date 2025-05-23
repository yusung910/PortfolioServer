#pragma once
#include <atomic>
#include "MDBDatas.h"

constexpr size_t MDB_DATA_SWITCH_COUNT = 3; //데이터 목록 스위칭

//MDB와 커넥션 생성하고 데이터를 가져오는 클래스
class MDBDataManager
{
private:
    static MDBDatas m_oMDBDatas[MDB_DATA_SWITCH_COUNT];
    static std::atomic_size_t m_nCurrentDataIdx;

public:
    ~MDBDataManager();
    static MDBDatas& GetInst();

    static MDBDatas& GetNextMDBDatas();
    static void Switch();

protected:
    MDBDataManager() = default;
};

