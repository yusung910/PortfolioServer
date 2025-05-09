#include "PGPPrivate.h"
#include "MDBDataManager.h"

MDBDatas            MDBDataManager::m_oMDBDatas[MDB_DATA_SWITCH_COUNT];
std::atomic_size_t  MDBDataManager::m_nCurrentDataIdx = 0;

MDBDataManager::~MDBDataManager()
{
    for (int i = 0; i < MDB_DATA_SWITCH_COUNT; i++)
    {
        m_oMDBDatas[i].Reset();
    }
}

MDBDatas& MDBDataManager::GetInst()
{
    return m_oMDBDatas[m_nCurrentDataIdx.load()];
}

MDBDatas& MDBDataManager::GetNextMDBDatas()
{
    size_t lNext = m_nCurrentDataIdx.load();
    lNext++;
    lNext %= MDB_DATA_SWITCH_COUNT;

    return m_oMDBDatas[lNext];
}

void MDBDataManager::Switch()
{
    size_t lNext = m_nCurrentDataIdx.load();
    lNext++;
    lNext %= MDB_DATA_SWITCH_COUNT;

    m_nCurrentDataIdx.store(lNext);
}
