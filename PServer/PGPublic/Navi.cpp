#include "PGPPrivate.h"
#include "Navi.h"
#include "Random.h"
#include "ServerConfig.h"
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
    for (auto lIt : m_umMapMeshList)
    {
        if (nullptr != lIt.second)
            SafeDelete(lIt.second);
    }

    m_umMapMeshList.clear();
}

bool Navi::Init(std::vector<MapLoadData> _mapInfo)
{
    if (true == m_bIsInit)
        return false;

    auto localStrMapFileDir = ServerConfig::GetInst().GetConfig().GetMapInfoPath();

    if (true == localStrMapFileDir.empty())
        SetLoadFileDir(localStrMapFileDir);

    int localSuccessCount = 0;

    int64_t localStartTime = Clock::GetTick64();

    for (auto mapData : _mapInfo)
    {
        int localMeshSize = std::max(mapData.m_nHeight, mapData.m_nWidth);

        if (false == _LoadMapMesh(mapData.m_nMapID, mapData.m_sNaviFileName, localMeshSize))
        {
            VIEW_WRITE_ERROR("Load Map Mesh Failed!! MapID : %d", mapData.m_nMapID);
        }
        else
        {
            localSuccessCount++;
        }
    }

    m_bIsInit = (localSuccessCount > 0);

    int64_t lGap = Clock::GetTick64() - localSuccessCount;
    VIEW_INFO("Load Maps : %d ms", lGap);

    return m_bIsInit;
}

bool Navi::GetRandomPositionAroundCircle(int _mapID, Position& _targetPos, float _rad, [[maybe_unused]] bool _setRad)
{
    auto lMesh = _FindMapMesh(_mapID);
    if (nullptr == lMesh)
        return false;

    bool lRslt = false;

    if (nullptr == lMesh->getNavMeshQuery())
        return lRslt;

    Position lRandomPos = _targetPos;

    //반경 내(Radius) 임의의 거리를 구한다
    //0~1 사이 임의의 실수(float)의 절대값 제곱근 * 반경(_rad)
    float lRandomDist = sqrt(fabsf(Random::GetInst()->GetRandomRange(0.f, 1.0f))) * _rad;

    float lRot = DegreeToRadian(Random::GetInst()->GetRandomRange(0, 360));

    lRandomPos.x = _targetPos.x + (lRandomDist * cosf(lRot));
    lRandomPos.y = _targetPos.y + (lRandomDist * sinf(lRot));

    //
    for (int i = 0; i < 8; i++)
    {
        if (IsMovePos(_mapID, lRandomPos))
        {
            lRslt = true;
            break;
        }
        lRot += 45.0f;
        lRandomPos.x = _targetPos.x + (lRandomDist * cosf(lRot));
        lRandomPos.y = _targetPos.y + (lRandomDist * sinf(lRot));
    }

    if (lRslt)
        _targetPos = lRandomPos;

    return lRslt;
}

bool Navi::IsMovePos(int _mapID, const Position& _startPos, float* lHeight)
{
    auto lMesh = _FindMapMesh(_mapID);

    if (nullptr == lMesh)
        return false;
    if (nullptr == lMesh->getNavMeshQuery())
        return false;

    float lPos[3], lNearPos[3];

    _ConvertToDetour(_startPos, lPos);
    _ConvertToDetour(_startPos, lNearPos);

    auto lQuery = lMesh->getNavMeshQuery();

    dtPolyRef lPolyStart = 0;
    dtCrowd* lCrowd = lMesh->getCrowd();

    const dtQueryFilter* lFilter = lCrowd->getFilter(0);
    const float* lHalfExtents = lCrowd->getQueryExtents();
    float lAgentPlacementHalfExtents[3] = { lHalfExtents[0], lHalfExtents[1] * NAVI_HEIGHT_CHECK_PICK_SERVER, lHalfExtents[2] };

    dtStatus lStatus = lQuery->findNearestPoly(lPos, lAgentPlacementHalfExtents, lFilter, &lPolyStart, lNearPos);
    
    if (dtStatusFailed(lStatus))
        return false;

    float lTmp = 0.0f;
    lStatus = lQuery->getPolyHeight(lPolyStart, lPos, &lTmp);

    if (dtStatusFailed(lStatus))
        return false;

    if (nullptr != lHeight)
        *lHeight = lTmp;

    return true;
}

bool Navi::SetLoadFileDir(const std::string& _dir)
{
    if (true == _dir.empty()) return false;

    m_sLoadFileDir = _dir;
    return true;
}

bool Navi::Raycast(const int& _mapID, const Position& _currentPos, const Position& _targetPos)
{
    return false;
}

bool Navi::_LoadMapMesh(const int& _MapID, const std::string&
    _strFileName, const int& _meshSize)
{
    if (auto lIt = m_umMapMeshList.find(_MapID); lIt != m_umMapMeshList.end())
        return false;

    if (auto lIt = m_umFileNameMeshMap.find(_strFileName); lIt != m_umFileNameMeshMap.end())
    {
        m_umMapMeshList.insert_or_assign(_MapID, lIt->second);
        return true;
    }

    // 파일 검사
    auto lStrLoadFilePath = FileSystem::GetExecutableFileName() + m_sLoadFileDir + _strFileName;

    std::error_code lEC;
    auto lFilePath = std::filesystem::path(StringUtil::ToWideChar(lStrLoadFilePath));

    if (false == std::filesystem::exists(lFilePath, lEC))
    {
        lStrLoadFilePath = m_sLoadFileDir + _strFileName;
        if (false == std::filesystem::exists(lFilePath, lEC))
            return false;
    }

    if (false == std::filesystem::is_regular_file(lFilePath, lEC))
        return false;

    SampleTest* lMesh = new SampleTest;
    lMesh->loadAll(lStrLoadFilePath.c_str());

    //
    auto lRetStat = lMesh->getNavMeshQuery()->init(lMesh->getNavMesh(), _meshSize);
    _SetNavCrowd(lMesh);

    m_umMapMeshList.insert(std::pair(_MapID, lMesh));
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

SampleTest* Navi::_FindMapMesh(int _mapID)
{
    if (auto it = m_umMapMeshList.find(_mapID); it != m_umMapMeshList.end())
        return it->second;
    return nullptr;
}

void Navi::_GetNormalVector2D(Position& _pos)
{
    auto lLen = _GetLengthXY(_pos);
    if (lLen == 0.0f)
        return;

    _pos.x /= lLen;
    _pos.y /= lLen;
    _pos.z = 0;
}

float Navi::_GetLengthXY(Position& _pos)
{
    return sqrt(_pos.x * _pos.x + _pos.y * _pos.y);
}

void Navi::_ConvertToDetour(const Position& _pos, float* _xyz)
{
    _xyz[0] = _pos.x;
    _xyz[2] = -_pos.y;
    _xyz[1] = _pos.z;
}

void Navi::_RevertToDetour(const float* _xyz, Position& _pos)
{
    _pos.x = _xyz[0];
    _pos.y = -_xyz[2];
    _pos.z = _xyz[1];
}
