#pragma once

#include "GlobalConst.h"
#include "GlobalEnum.h"
#include "MasterDB.h"

#include <SecureValueClass.h>
#include <set>
#include <functional>

class ObjectStats
{

private:
    //
    using EquipID = int;

    SVint m_nBaseStats[EAbilityComponents::Max] = { 0, };
    SVint m_nAddedStats[EAbilityComponents::Max] = { 0, };
    SVint m_nAddedRateStats[EAbilityComponents::Max] = { 0, };

public:
    ObjectStats();
    ObjectStats(const ObjectStats& _stats);

    void SetObjectStats(const ObjectStats* _stats);

    void SetBaseAbilityStat(EAbilityComponents::Type _abilityType, int _val);

    void Reset();

};

