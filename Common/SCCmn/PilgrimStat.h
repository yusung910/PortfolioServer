#pragma once

#include "PGObject.h"
#include "MasterDB.h"

class PilgrimStat : public PGObject<PilgrimStat>
{
private:

    const MDBObjectStatistics* m_oPilgrimStat;

public:
    PilgrimStat(ObjectTag);
    ~PilgrimStat();

    void Reset();
    const MDBObjectStatistics* GetPilgrimStat();
    void Set(const MDBObjectStatistics* _pgStat);
};

