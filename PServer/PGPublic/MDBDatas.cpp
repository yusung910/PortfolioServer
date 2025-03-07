#include "PGPPrivate.h"
#include "MDBDatas.h"

#define MAP_CLEAR(x) { for(auto it = x.begin(); it != x.end(); ++it) SafeDelete(it->second); x.clear(); }

#define VECTOR_CLEAR(x) { for(auto it = x.begin(); it != x.end(); ++it) SafeDelete(*it); x.clear(); }

void MDBDatas::Reset()
{
    MAP_CLEAR(m_umMDBPilgrimStatList);
    MAP_CLEAR(m_umMDBMapInfoList);
}

std::unordered_map<int, MDBPilgrimStat*>& MDBDatas::GetAllPilgrimStatList()
{
    return m_umMDBPilgrimStatList;
}

std::unordered_map<int, MDBMapInfo*>& MDBDatas::GetAllMapInfoList()
{
    return m_umMDBMapInfoList;
}

std::unordered_map<int, MDBMapTeleportInfo*>& MDBDatas::GetAllMapTeleportInfoList()
{
    return m_umMDBMapTeleportInfoList;
}
