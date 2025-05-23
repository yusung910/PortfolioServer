#include "PGPPrivate.h"
#include "AreaManager.h"

AreaManager::AreaManager()
{
    Initialize();
}

AreaManager::~AreaManager()
{
    m_umAreas.clear();
}

void AreaManager::Initialize()
{
    m_umAreas.clear();
}

bool AreaManager::CreateArea(const std::vector<MapLoadData>& _mapData)
{
    if (false == m_umAreas.empty())
        return false;

    for (auto& data : _mapData)
        _CreateArea(data);

    return !m_umAreas.empty();
}

std::shared_ptr<Area> AreaManager::_CreateArea(const MapLoadData& _mapData)
{
    if (m_umAreas.find(_mapData.m_nMapID) != m_umAreas.end())
        return nullptr;


    Area::SharedPtr localArea = Area::New();

    if (false == localArea->Create(_mapData.m_nMapID, _mapData.m_nWidth, _mapData.m_nHeight))
    {
        localArea.reset();
        return nullptr;
    }

    m_umAreas.insert({ _mapData.m_nMapID, localArea });
    return localArea;
}
