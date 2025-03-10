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

    // ���� ���� ��ġ
    Position m_oChaseDetectedPos; 

    // ���� ���� ��ġ 
    Position m_oChaseUpdatePos; 

    // NPC�� ���� ������ �Ÿ�
    float m_fChasableDistance = 0.0f;

    // ��ȯ�� ���� ���
    bool m_bReturnDivineMode = false;

    // ��ȯ ��ġ ���� ����
    bool m_bReturnPosArrival = true;

    // ������ ���� Ÿ��
    int m_nLastTargetSeq = 0;

    //
    NPCStateEngine m_oStateEngine;


public:
    NPC(ObjectTag);
    virtual ~NPC();

    virtual void Reset();
};

