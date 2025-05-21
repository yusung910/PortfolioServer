#include "PGPPrivate.h"
#include "MDBDatas.h"

#define MAP_CLEAR(x) { for(auto it = x.begin(); it != x.end(); ++it) SafeDelete(it->second); x.clear(); }

#define VECTOR_CLEAR(x) { for(auto it = x.begin(); it != x.end(); ++it) SafeDelete(*it); x.clear(); }

const MDBMapInfo* MDBDatas::GetMapInfo(const int& _mapID)
{
    if (auto lIter = m_umMDBMapInfoList.find(_mapID); lIter != m_umMDBMapInfoList.end())
        return lIter->second;
    return nullptr;
}

void MDBDatas::Reset()
{
    MAP_CLEAR(m_umMDBObjectStatisticsList);
    MAP_CLEAR(m_umMDBMapInfoList);
}

std::unordered_map<int, MDBObjectStatistics*>& MDBDatas::GetAllMDBObjectStatisticsList()
{
    return m_umMDBObjectStatisticsList;
}

std::unordered_map<int, MDBMapInfo*>& MDBDatas::GetAllMapInfoList()
{
    return m_umMDBMapInfoList;
}

std::unordered_map<int, MDBMapTeleportInfo*>& MDBDatas::GetAllMapTeleportInfoList()
{
    return m_umMDBMapTeleportInfoList;
}
