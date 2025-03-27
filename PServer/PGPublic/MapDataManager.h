#pragma once
#include <RefSingleton.h>
#include "MapLoadData.h"
#include "MapElementsData.h"

struct MDBMapInfo;

constexpr const char* MAP_NAVI_FILE_EXT = ".bin";

class MapElementsData;

class MapDataManager :public RefSingleton<MapDataManager>
{
private:
    std::string m_sMapFilePath = "";
    MapLoadList m_oMapFileList;

    std::unordered_map<int, std::unordered_map<int, MapElementsData*>> mMapElementsDatas;

public:
    bool Init();
};

