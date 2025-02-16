#pragma once
#include <Position.h>
#include "Direction.h"
#include <array>
#include <variant>

constexpr float g_fDefaultMoveSpeed = 5.f;
constexpr float g_fDefaultAttackSpeed = 1.f;

class BaseObject
{
protected:
    int                 m_nSequenceID = 0;       //메모리 적재된 오브젝트 번호
    int                 m_nTemplateID = 0;       //

    std::string         m_sName;                 //UTF-8
    ObjectStats         m_oObjStats;

    int                 m_nHP;
    int                 m_nMana;

    int                 m_nLevel;
    int64_t             m_nExp;
    Direction           m_oDirection;
    Position            m_oPosition;
    
    bool                m_bIsMove = false;

public:
    BaseObject() = default;
    virtual ~BaseObject() = default;


    //---------------------------------------
    // Virtual Functions
    virtual void Reset();
    virtual void Update(const int64_t& _inDeltaSec);
    virtual bool IsPreemptionTarget() { return false; }



};

