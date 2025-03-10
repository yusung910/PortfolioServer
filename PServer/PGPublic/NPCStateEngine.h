#pragma once
#include "ENPCState.h"
#include <unordered_map>

class NPC;
class IState;

class NPCStateEngine
{
private:
    NPC& m_oNPC;

    ENPCState m_eCurrentState = ENPCState::None;
    std::unordered_map<ENPCState, IState*> m_umStateList;

    IState* m_pCurrentState = nullptr;

    bool m_bIsInitialized = false;

public:
    NPCStateEngine(NPC& _npc);
    ~NPCStateEngine();

    void Init();
    void Finish();
    bool ChangeState(ENPCState _s, const int64_t& _tick);
};

