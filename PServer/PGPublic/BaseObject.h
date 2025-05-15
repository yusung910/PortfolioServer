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
    int                 m_nSequence = 0;       //메모리 적재된 오브젝트 번호
    int                 m_nNPCUniqueID = 0;      //

    std::string         m_sName;                 //UTF-8
    ObjectStats         m_oObjStats;

    int                 m_nHP = 0;
    int                 m_nMana = 0;

    int                 m_nLevel = 0;
    int64_t             m_nExp = 0;
    Direction           m_oDirection;
    Position            m_oPosition;
    
    bool                m_bIsMove = false;

    EObject::Type       m_eObjectType = EObject::None;

public:
    BaseObject() = default;

    virtual ~BaseObject() = default;

    //---------------------------------------
    // Virtual Functions
    virtual void Reset();
    virtual void Update(const int64_t& _inDeltaSec);
    virtual bool IsPreemptionTarget() { return false; }

    void SetNPCUniqueID(const int& _id);

    const int& GetSequence();
    void SetSequence(const int& _seq);
    //---------------------------------------

    EObject::Type GetObjectType();
    void SetObjectType(EObject::Type _t);
};

