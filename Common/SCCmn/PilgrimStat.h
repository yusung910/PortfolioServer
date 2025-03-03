#pragma once

#include "PGObject.h"
#include "MasterDB.h"

class PilgrimStat : public PGObject<PilgrimStat>
{
private:

    const MDBPilgrimStat* m_oPilgrimStat;

public:
    PilgrimStat(ObjectTag);
    ~PilgrimStat();

    void Reset();
    const MDBPilgrimStat* GetPilgrimStat();
    void Set(const MDBPilgrimStat* _pgStat);
};

