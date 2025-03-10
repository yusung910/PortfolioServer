#include "PGPPrivate.h"
#include "Navi.h"
#include "Random.h"
#include "StringUtil.h"
#include "FileSystem.h"

#include <DetourCommon.h>
#include <MathUtil.h>
#include <Clock.h>

Navi::Navi()
{
}

Navi::~Navi()
{
    for (auto lIt : m_umZoneMeshList)
    {
        if (nullptr != lIt.second)
            SafeDelete(lIt.second);
    }

    m_umZoneMeshList.clear();
}

bool Navi::Init(std::vector<MDBMapInfo*>* _mapInfo)
{
    if (true == m_bIsInit)
        return false;

    if (nullptr == _mapInfo)
        return false;

    int lSuccessCount = 0;

    int64_t lStartTime = Clock::GetTick64();
    
    for (auto lIter : *_mapInfo)
    {
        int lMeshSize = std::max(lIter->MapHeight, lIter->MapWidth);

    }

    return false;
}

bool Navi::SetLoadFileDir(const std::string& _dir)
{
    if (true == _dir.empty()) return false;

    m_sLoadFileDir = _dir;
    return true;
}

bool Navi::_LoadZoneMesh(const int& _MapID, const std::string&
    _strFileName, const int& _meshSize)
{
    if (auto lIt = m_umZoneMeshList.find(_MapID); lIt != m_umZoneMeshList.end())
        return false;

    if (auto lIt = m_umFileNameMeshMap.find(_strFileName); lIt != m_umFileNameMeshMap.end())
    {
        m_umZoneMeshList.insert_or_assign(_MapID, lIt->second);
        return true;
    }

    // 파일 검사
    auto lStrLoadFilePath = FileSystem::GetExecutableFileName() + m_sLoadFileDir + _strFileName;

    std::error_code lEC;
    auto lFilePath = std::filesystem::path(StringUtil::ToWideChar(lStrLoadFilePath));

    if (false == std::filesystem::exists(lFilePath, lEC))
    {
        lStrLoadFilePath = m_sLoadFileDir + _strFileName;
        if(false == std::filesystem::exists(lFilePath, lEC))
            return false;
    }

    if (false == std::filesystem::is_regular_file(lFilePath, lEC))
        return false;

    SampleTest* lMesh = new SampleTest;
    lMesh->loadAll(lStrLoadFilePath.c_str());

    //
    auto lRetStat = lMesh->getNavMeshQuery()->init(lMesh->getNavMesh(), _meshSize);
    _SetNavCrowd(lMesh);

    m_umZoneMeshList.insert(std::pair(_MapID, lMesh));
    m_umFileNameMeshMap.insert_or_assign(_strFileName, lMesh);
    return true;
}

void Navi::_SetNavCrowd(SampleTest* _mesh)
{
    if (nullptr == _mesh)
        return;

    dtNavMesh* lNav = _mesh->getNavMesh();
    dtCrowd* lCrowd = _mesh->getCrowd();

    if (nullptr != lNav && nullptr != lCrowd)
    {
        lCrowd->init(MAX_AGENTS_SERVER, _mesh->getAgentRadius(), lNav);

        //
        lCrowd->getEditableFilter(0)->setExcludeFlags(SAMPLE_POLYFLAGS_DISABLED);

        //
        dtObstacleAvoidanceParams lParams;
        //
        memcpy(&lParams, lCrowd->getObstacleAvoidanceParams(0), sizeof(dtObstacleAvoidanceParams));

        for (int i = 0; i < DT_CROWD_MAX_OBSTAVOIDANCE_PARAMS; i++)
        {
            lParams.velBias = 1.0f;
            lParams.velBias = 0.4f;
            lParams.weightDesVel = 2.0f;
            lParams.weightCurVel = 0.75f;
            lParams.weightSide = 0.75f;
            lParams.weightToi = 2.5f;
            lParams.horizTime = 2.5f;
            lParams.gridSize = 33;
            lParams.adaptiveDivs = 7;
            lParams.adaptiveRings = 2;
            lParams.adaptiveDepth = 5;
            if (i == 0)
            {
                lParams.velBias = 0.5f;
                lParams.adaptiveDivs = 5;
                lParams.adaptiveRings = 2;
                lParams.adaptiveDepth = 1;
            }
            else if (i == 1)
            {
                lParams.velBias = 0.5f;
                lParams.adaptiveDivs = 5;
                lParams.adaptiveRings = 2;
                lParams.adaptiveDepth = 2;
                lCrowd->setObstacleAvoidanceParams(1, &lParams);
            }
            else if (i == 2)
            {
                lParams.velBias = 0.5f;
                lParams.adaptiveDivs = 7;
                lParams.adaptiveRings = 2;
                lParams.adaptiveDepth = 3;
            }
            else if (i == 3)
            {
                lParams.velBias = 0.5f;
                lParams.adaptiveDivs = 7;
                lParams.adaptiveRings = 3;
                lParams.adaptiveDepth = 3;
            }
            lCrowd->setObstacleAvoidanceParams(i, &lParams);
        }


        bool lUseFilter = false;
        if (lCrowd && lUseFilter)
        {
            //dtQueryFilter* filter = lCrowd->getEditableFilter(0);
            //filter->setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);
            //filter->setAreaCost(SAMPLE_POLYAREA_WATER, 1.0f);
            //filter->setAreaCost(SAMPLE_POLYAREA_ROAD, 0.0000001f);
            //filter->setAreaCost(SAMPLE_POLYAREA_DOOR, 1.0f);
            //filter->setAreaCost(SAMPLE_POLYAREA_GRASS, 1000000.0f);
            //filter->setAreaCost(SAMPLE_POLYAREA_JUMP, 1.0f);
            //filter->setIncludeFlags(SAMPLE_POLYFLAGS_WALK);
            //filter->setExcludeFlags(SAMPLE_POLYFLAGS_SWIM);

            //dtQueryFilter* filter2 = lCrowd->getEditableFilter(1);
            //float fDefault = 1.0f;
            //*filter2 = *filter;
            //filter2->setIncludeFlags(SAMPLE_POLYFLAGS_ALL);
            //filter2->setExcludeFlags(0);

            //filter2->setAreaCost(SAMPLE_POLYAREA_GROUND, fDefault);
            //filter2->setAreaCost(SAMPLE_POLYAREA_WATER, fDefault);
            //filter2->setAreaCost(SAMPLE_POLYAREA_ROAD, fDefault);
            //filter2->setAreaCost(SAMPLE_POLYAREA_DOOR, fDefault);
            //filter2->setAreaCost(SAMPLE_POLYAREA_GRASS, fDefault);
            //filter2->setAreaCost(SAMPLE_POLYAREA_JUMP, fDefault);
        }
    }
}
