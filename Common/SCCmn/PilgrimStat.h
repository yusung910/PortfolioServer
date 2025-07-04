#pragma once

#include "PGObject.h"
#include "MasterDB.h"

class PilgrimStat : public PGObject<PilgrimStat>
{
private:

    const MDBAbilityComponents* m_oAbilityComponents;

public:
    PilgrimStat(ObjectTag);
    ~PilgrimStat();

    void Reset();
    const MDBAbilityComponents* GetAbilityComponent();
    void Set(const MDBAbilityComponents* _pgStat);
};

