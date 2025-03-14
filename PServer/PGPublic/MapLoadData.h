#pragma once
#include <string>
#include <vector>
#include <Position.h>
#include <GlobalEnum.h>


class MapLoadData
{
public:
    int m_nMapID = 0;
    int m_nParentMapID = 0;
    EMap::Type m_eMapType = EMap::None;
    
    std::string m_sNaviFileName;

    int m_nWidth = 0;
    int m_nHeight = 0;

    Position m_oCenterPosition;
};

typedef std::vector<MapLoadData> MapLoadList;