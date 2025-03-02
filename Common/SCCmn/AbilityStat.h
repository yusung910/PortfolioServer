#pragma once

#include "PGObject.h"
#include "MasterDB.h"

class AbilityStat : public PGObject<AbilityStat>
{
private:

    const MDBAbilityStat* m_oAbilityStat;

public:
    AbilityStat(ObjectTag);
    ~AbilityStat();

    void Reset();
    const MDBAbilityStat* GetAbilityStat();
};

