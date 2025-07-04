#include "SCCmnPrivate.h"
#include "ObjectStats.h"

#ifdef SERVER_BUILD
#	pragma warning(push)
#	pragma warning(disable : 4819)
#endif


ObjectStats::ObjectStats()
{
}

ObjectStats::ObjectStats(const ObjectStats& _stats)
{
    Reset();
    SetObjectStats(&_stats);
}

void ObjectStats::SetObjectStats(const ObjectStats* _stats)
{
    Reset();
    *this = *_stats;
}

void ObjectStats::SetBaseAbilityStat(EAbilityComponents::Type _abilityType, int _val)
{
    if (EAbilityComponents::IsValid(_abilityType))
    {
        m_nBaseStats[_abilityType] = _val;
    }
}

void ObjectStats::Reset()
{
    for (int i = 0; i < EAbilityComponents::Max; i++)
    {
        m_nBaseStats[i] = 0;
        m_nAddedStats[i] = 0;
        m_nAddedRateStats[i] = 0;
    }
}
