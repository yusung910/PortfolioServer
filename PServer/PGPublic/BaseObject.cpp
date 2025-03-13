#include "PGPPrivate.h"
#include "BaseObject.h"

void BaseObject::Reset()
{
    m_nSequenceID = 0;       //메모리 적재된 오브젝트 번호
    m_nTemplateID = 0;       //

    m_sName.clear();                 //UTF-8
    m_oObjStats.Reset();

    m_nHP = 0;
    m_nMana = 0;

    m_nLevel = 0;
    m_nExp = 0;
    m_oDirection.Reset();
    m_oPosition.Reset();

    m_bIsMove = false;
}

void BaseObject::Update([[maybe_unused]]const int64_t& _inDeltaSec)
{
}

void BaseObject::SetNPCUniqueID(const int& _id)
{
    m_nNPCUniqueID = _id;
}

void BaseObject::SetSequence(const int& _seq)
{
    m_nSequence = _seq;
}
