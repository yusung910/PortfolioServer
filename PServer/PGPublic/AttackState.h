#pragma once
#include "IState.h"

class AttackState : public IState
{
public:
    AttackState(NPC& _npc);
    virtual ~AttackState() = default;

    virtual void Start(const int64_t& _tick) override;
    virtual void Update(const int64_t& _tick);

private:
};

