#pragma once
#include <atomic>
#include "MDBDatas.h"

constexpr size_t MDB_DATA_SWITCH_COUNT = 3; //������ ��� ����Ī

//MDB�� Ŀ�ؼ� �����ϰ� �����͸� �������� Ŭ����
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

