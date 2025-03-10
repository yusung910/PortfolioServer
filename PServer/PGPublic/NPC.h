#pragma once
#include "BaseObject.h"
#include "PGObject.h"
#include "Navi.h"

// module
#include "NPCStateEngine.h"

struct MDBNPC;

class NPC : public BaseObject, public PGObject<NPC>
{
private:
    //
    stNaviInfoServer m_oNaviInfo;

    // 추적 감지 위치
    Position m_oChaseDetectedPos; 

    // 추적 갱신 위치 
    Position m_oChaseUpdatePos; 

    // NPC가 추적 가능한 거리
    float m_fChasableDistance = 0.0f;

    // 귀환시 무적 모드
    bool m_bReturnDivineMode = false;

    // 소환 위치 도착 여부
    bool m_bReturnPosArrival = true;

    // 마지막 공격 타겟
    int m_nLastTargetSeq = 0;

    //
    NPCStateEngine m_oStateEngine;


public:
    NPC(ObjectTag);
    virtual ~NPC();

    virtual void Reset();
};

