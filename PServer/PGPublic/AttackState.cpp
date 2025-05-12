#include "PGPPrivate.h"
#include "AttackState.h"

AttackState::AttackState(NPC& _npc)
    :IState(ENPCState::Attack, _npc)
{
}

void AttackState::Start(const int64_t& _tick)
{
    m_nStateTick = 0;
    Update(_tick);
}

void AttackState::Update(const int64_t& _tick)
{
}
