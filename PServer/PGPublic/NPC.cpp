#include "PGPPrivate.h"
#include "NPC.h"

NPC::NPC(ObjectTag)
    : m_oStateEngine(*this), m_oStat(*this)
{
    m_oStateEngine.Init();
}

NPC::~NPC()
{
}

void NPC::Reset()
{
    BaseObject::Reset();
    m_oNaviInfo.Clear();
    m_oStateEngine.ChangeState(ENPCState::Idle, Clock::GetTick64());

    m_fChasableDistance = 0.0f;

    m_bReturnDivineMode = false;
    m_bReturnPosArrival = true;

}

void NPC::SetObjectStats(const MDBAbilityModule* _objStats)
{
    if (nullptr != _objStats)
    {
        
    }
}
