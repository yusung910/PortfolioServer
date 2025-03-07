#pragma once

#include <Position.h>
#include <RefSingleton.h>
#include <unordered_map>


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

    std::unordered_map <int, SampleTest*> m_umZoneMeshList;
    std::unordered_map <std::string, SampleTest*> m_umFileNameMeshMap;

    stNaviInfoServer m_oNaviRaycast;
    stNaviInfoServer m_oTeleportNaviInfo;

public:
    Navi();
    virtual ~Navi();

    bool Init(std::vector<MDBMapInfo*>* _mapInfo);


private:
    bool _LoadZoneMesh(const int& _MapID, const int& _meshSize);

};