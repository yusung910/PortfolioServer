#include "SCCmnPrivate.h"
#include "AbilityStat.h"

AbilityStat::AbilityStat(ObjectTag)
{
    Reset();
}

AbilityStat::~AbilityStat()
{
}

void AbilityStat::Reset()
{
    m_oAbilityStat = nullptr;
}

const MDBAbilityStat* AbilityStat::GetAbilityStat()
{
    return m_oAbilityStat;
}
