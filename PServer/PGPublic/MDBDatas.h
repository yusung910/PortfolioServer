#pragma once
#include <MasterDB.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

class MDBDatas
{
public:
    std::unordered_map<int, MDBHeroStat*> m_umMDBHeroStatList;
    std::unordered_map<int, MDBMapInfo*> m_umMDBMapInfoList;
    std::unordered_map<int, MDBMapTeleportInfo*> m_umMDBMapTeleportInfoList;


public:
    std::unordered_map<int, MDBHeroStat*>& GetAllHeroStatList();
    std::unordered_map<int, MDBMapInfo*>& GetAllMapInfoList();
    std::unordered_map<int, MDBMapTeleportInfo*>& GetAllMapTeleportInfoList();

    void Reset();
};

