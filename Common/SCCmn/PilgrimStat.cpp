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

const MDBObjectStatistics* PilgrimStat::GetPilgrimStat()
{
    return m_oPilgrimStat;
}

void PilgrimStat::Set(const MDBObjectStatistics* _pgStat)
{
    m_oPilgrimStat = _pgStat;
}
