#pragma once

#include <Position.h>
#include <RefSingleton.h>
#include <unordered_map>

#include "MapLoadData.h"

#include <DetourCrowd.h>
#include <Sample_TempObstacles.h>

#define MAX_POLYS_SERVER 64
#define MAX_AGENTS_SERVER 1
#define NAVI_HEIGHT_CHECK_PICK_SERVER 100.0f

struct _PathInfoServer
{
    float m_fStartPos[3];
    float m_fEndPos[3];
    const dtQueryFilter* m_oFilter;
    dtNavMeshQuery* m_oNavQuery;
};

struct stNaviInfoServer
{
public:
    dtPolyRef m_oPolysStart;
    dtPolyRef m_oPolysEnd;
    int m_nPolysCount;
    dtPolyRef m_oPolys[MAX_POLYS_SERVER];

    float m_fTargetNaviLengthListTotalSum;
    std::vector<float> m_vTargetNaviLengthList;
    std::vector<Position> m_vTargetNaviPositionList;
    float m_fLastMove;

    stNaviInfoServer()
    {
        Clear();
    }

    void Clear()
    {
        m_oPolysStart = 0;
        m_oPolysEnd = 0;
        m_nPolysCount = 0;
        memset(m_oPolys, 0, sizeof(m_oPolys));

        ClearNavi();
    }

    void ClearNavi()
    {
        m_fTargetNaviLengthListTotalSum = 0.0f;
        m_vTargetNaviLengthList.clear();
        m_vTargetNaviPositionList.clear();

        m_fLastMove = 0.0f;
    }

    //각각 Position(좌표)값들간의 거리를 구해서 list에 저장하고
    //각각 거리의 합산을 반환하는 함수
    float SetTargetFullLength()
    {
        m_fTargetNaviLengthListTotalSum = 0.0f;
        m_vTargetNaviLengthList.clear();
        m_vTargetNaviLengthList.push_back(0);

        float lDist = 0.0f;

        if (m_vTargetNaviLengthList.size() > 0)
        {
            for (size_t i = 0; i < m_vTargetNaviPositionList.size() - 1; i++)
            {
                lDist = m_vTargetNaviPositionList[i].GetDistance(m_vTargetNaviPositionList[i + 1]);

                m_vTargetNaviLengthList.push_back(lDist);
                m_fTargetNaviLengthListTotalSum += lDist;
            }
        }
        return m_fTargetNaviLengthListTotalSum;
    }
};


typedef Sample_TempObstacles SampleTest;

class Navi : public RefSingleton<Navi>
{
private:
    bool m_bIsInit = false;

    std::unordered_map <int, SampleTest*> m_umMapMeshList;
    std::unordered_map <std::string, SampleTest*> m_umFileNameMeshMap;
    std::string m_sLoadFileDir = "";


    stNaviInfoServer m_oNaviRaycast;
    stNaviInfoServer m_oTeleportNaviInfo;

public:
    Navi();
    virtual ~Navi();

    bool Init(std::vector<MapLoadData> _mapInfo);

    bool GetRandomPositionAroundCircle(int _mapID, Position& _targetPos, float _rad, bool _setRad = false);

    bool IsMovePos(int _mapID, const Position& _startPos, float* lHeight = NULL);

    bool SetLoadFileDir(const std::string& _dir);


    //충돌 여부 확인
    //False = 충돌, true = 충돌 없음
    bool Raycast(const int& _mapID, const Position& _currentPos, const Position& _targetPos);

private:
    bool _LoadMapMesh(const int& _MapID, const std::string&
        _strFileName, const int& _meshSize);
    void _SetNavCrowd(SampleTest* _mesh);

    SampleTest* _FindMapMesh(int _mapID);

    //인자값으로 전달받은 각각의 좌표를 거리값만큼 나누고 z값은 0으로 세팅하는 함수
    void _GetNormalVector2D(Position& _pos);

    //원점 좌표(0,0)에서 부터 _pos의 좌표 값 까지의 거리
    //sqrt(x^2 + y^2) -> 피타고라스 정리
    float _GetLengthXY(Position& _pos);

    void _ConvertToDetour(const Position& _pos, float* _xyz);
    void _RevertToDetour(const float* _xyz, Position& _pos);
};