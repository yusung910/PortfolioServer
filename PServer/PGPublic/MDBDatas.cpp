#include "PGPPrivate.h"
#include "MDBDatas.h"

#define MAP_CLEAR(x) { for(auto it = x.begin(); it != x.end(); ++it) SafeDelete(it->second); x.clear(); }

#define VECTOR_CLEAR(x) { for(auto it = x.begin(); it != x.end(); ++it) SafeDelete(*it); x.clear(); }


void MDBDatas::Reset()
{
    MAP_CLEAR(m_umMDBAbilityComponentsList);
    MAP_CLEAR(m_umMDBMapInfoList);
    MAP_CLEAR(m_umMDBAbilityModuleList);
    VECTOR_CLEAR(m_umMDBPilgrimLevelStatusList);
}

std::unordered_map<int, MDBAbilityComponents*>& MDBDatas::GetAllMDBAbilityComponentsList()
{
    return m_umMDBAbilityComponentsList;
}

std::unordered_map<int, MDBAbilityModule*>& MDBDatas::GetAllMDBAbilityModuleList()
{
    return m_umMDBAbilityModuleList;
}

std::vector<MDBPilgrimLevelStatus*>& MDBDatas::GetAllPilgrimLevelStatusList()
{
    return m_umMDBPilgrimLevelStatusList;
}

std::unordered_map<int, MDBMapInfo*>& MDBDatas::GetAllMapInfoList()
{
    return m_umMDBMapInfoList;
}

std::unordered_map<int, MDBMapTeleportInfo*>& MDBDatas::GetAllMapTeleportInfoList()
{
    return m_umMDBMapTeleportInfoList;
}

const MDBMapInfo* MDBDatas::GetMapInfo(const int& _mapID)
{
    if (auto lIter = m_umMDBMapInfoList.find(_mapID); lIter != m_umMDBMapInfoList.end())
        return lIter->second;
    return nullptr;
}