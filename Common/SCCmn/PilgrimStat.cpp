#include "SCCmnPrivate.h"
#include "PilgrimStat.h"

PilgrimStat::PilgrimStat(ObjectTag)
{
    Reset();
}

PilgrimStat::~PilgrimStat()
{
}

void PilgrimStat::Reset()
{
    m_oAbilityComponents = nullptr;
}

const MDBAbilityComponents* PilgrimStat::GetAbilityComponent()
{
    return m_oAbilityComponents;
}

void PilgrimStat::Set(const MDBAbilityComponents* _pgStat)
{
    m_oAbilityComponents = _pgStat;
}
