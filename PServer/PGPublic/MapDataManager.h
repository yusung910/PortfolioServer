#pragma once
#include <RefSingleton.h>
#include "MapLoadData.h"

struct MDBMapInfo;

constexpr const char* MAP_NAVI_FILE_EXT = ".bin";
constexpr const char* MAP_INFO_FILE_EXT = ".map.json";
constexpr const char* MAP_SPAWN_FILE_EXT = ".spawn.json";

class MapElementsData;

class MapDataManager :public RefSingleton<MapDataManager>
{
public:
    MapDataManager() = default;
    virtual ~MapDataManager();

    bool Initialize();

private:
    std::string m_sMapFilePath = "";
    std::vector<MapLoadData> m_oMapFileList;

    std::unordered_map<int, std::unordered_map<int, MapElementsData*>> m_umMapElementsDatas;

    std::unordered_map<int, int> m_umMapChannelCountList;
    std::unordered_set<int> m_umHasChannelMapList;


    
private:
    bool _AddData(MDBMapInfo* _info);
    bool _SetMapInfo(MapLoadData& _data);

    MapElementsData* _GetMapElementsData(int _mapID, int _layerNum);
    bool _HasMapElementsDataList(int _mapID);

};

