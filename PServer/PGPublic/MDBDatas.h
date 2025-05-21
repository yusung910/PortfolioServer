#pragma once
#include <MasterDB.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

class MDBDatas
{
public:
    std::unordered_map<int, MDBObjectStatistics*> m_umMDBObjectStatisticsList;
    std::unordered_map<int, MDBMapInfo*> m_umMDBMapInfoList;
    std::unordered_map<int, MDBMapTeleportInfo*> m_umMDBMapTeleportInfoList;


public:
    //list
    std::unordered_map<int, MDBObjectStatistics*>& GetAllMDBObjectStatisticsList();
    std::unordered_map<int, MDBMapInfo*>& GetAllMapInfoList();
    std::unordered_map<int, MDBMapTeleportInfo*>& GetAllMapTeleportInfoList();


    //find Item
    const MDBMapInfo* GetMapInfo(const int& _mapID);

    void Reset();
};

