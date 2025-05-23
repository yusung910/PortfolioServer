#pragma once
#include <MasterDB.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

class MDBDatas
{
public:
    std::unordered_map<int, MDBAbilityComponents*> m_umMDBAbilityComponentsList;
    std::unordered_map<int, MDBAbilityModule*> m_umMDBAbilityModuleList;
    std::vector<MDBPilgrimLevelStatus*> m_umMDBPilgrimLevelStatusList;


    std::unordered_map<int, MDBMapInfo*> m_umMDBMapInfoList;
    std::unordered_map<int, MDBMapTeleportInfo*> m_umMDBMapTeleportInfoList;


public:
    //list
    std::unordered_map<int, MDBAbilityComponents*>& GetAllMDBAbilityComponentsList();
    std::unordered_map<int, MDBAbilityModule*>& GetAllMDBAbilityModuleList();
    std::vector<MDBPilgrimLevelStatus*>& GetAllPilgrimLevelStatusList();

    std::unordered_map<int, MDBMapInfo*>& GetAllMapInfoList();
    std::unordered_map<int, MDBMapTeleportInfo*>& GetAllMapTeleportInfoList();


    //find Item
    const MDBMapInfo* GetMapInfo(const int& _mapID);

    void Reset();
};

