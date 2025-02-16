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

    SVint m_nBaseStats[EStats::Max] = { 0, };

public:
    ObjectStats();
    ObjectStats(const ObjectStats& _stats);

    void SetObjectStats(const ObjectStats* _stats);
    void Reset();

};

