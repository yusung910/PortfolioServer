#pragma once
#include "ENPCState.h"
#include <cstdint>

constexpr int64_t NPC_MOVE_DELAY_TICK_MS = static_cast<int64_t>(PG_N_ZONE_SYNC_INTERVAL);

class NPC;

class IState
{
protected:
    NPC& m_oNPC;
    ENPCState m_eState = ENPCState::None;
    int64_t m_nStateTick = 0;


public:
    IState(const ENPCState& _state, NPC& _npc)
        : m_oNPC(_npc)
    {
        m_eState = _state;
    }

    virtual ~IState() = default;

    void UpdateTick(const int64_t& _t) noexcept
    {
        m_nStateTick = _t;
    }

    const ENPCState GetStateEngine() const noexcept
    {
        return m_eState;
    }

    virtual void Start(const int64_t& _t)
    {
        m_nStateTick = _t;
    }

    virtual void Update(const int64_t& _t) = 0;

    virtual void End([[maybe_unused]] const int64_t _t) { }
};