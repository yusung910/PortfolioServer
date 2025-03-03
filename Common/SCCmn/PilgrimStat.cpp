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
    m_oPilgrimStat = nullptr;
}

const MDBPilgrimStat* PilgrimStat::GetPilgrimStat()
{
    return m_oPilgrimStat;
}

void PilgrimStat::Set(const MDBPilgrimStat* _pgStat)
{
    m_oPilgrimStat = _pgStat;
}
