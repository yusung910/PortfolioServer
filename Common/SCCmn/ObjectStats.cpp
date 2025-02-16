#include "SCCmnPrivate.h"
#include "ObjectStats.h"

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

void ObjectStats::Reset()
{
    for (int i = 0; i < EStats::Max; i++)
    {
        m_nBaseStats[EStats::None] = 0;
    }
}
