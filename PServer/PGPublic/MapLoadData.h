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
    std::string m_sMapInfoFileName;
    std::string m_sSpawnFileName;


    int m_nWidth = 0;
    int m_nHeight = 0;

    Position m_oCenterPosition;
};


//±ª¿Ã « ø‰«—∞°..? «Ú∞•∏Æ¥Ÿ :: 250509
//typedef std::vector<MapLoadData> MapLoadList;