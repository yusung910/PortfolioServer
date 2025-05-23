#pragma once

#include <RefSingleton.h>
#include "MapLoadData.h"
#include "Area.h"
class NPC;
class BaseObject;

class AreaManager : public RefSingleton<AreaManager>
{
private:
    std::unordered_map<int, std::shared_ptr<Area>> m_umAreas;

    std::map<int, int> m_mAreaCCUData;
    std::mutex m_xCCULock;

    int64_t m_nLastCCUUpdateTick = 0;

public:
    AreaManager();
    virtual ~AreaManager();

    virtual void Initialize();

    bool CreateArea(const std::vector<MapLoadData>& _mapData);

private:
    std::shared_ptr<Area> _CreateArea(const MapLoadData& _mapData);
};

