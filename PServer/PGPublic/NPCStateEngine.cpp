#include "PGPPrivate.h"
#include "NPCStateEngine.h"

NPCStateEngine::NPCStateEngine(NPC& _npc)
    : m_oNPC(_npc)
{
}

NPCStateEngine::~NPCStateEngine()
{
}

void NPCStateEngine::Init()
{
}

void NPCStateEngine::Finish()
{
}

bool NPCStateEngine::ChangeState(ENPCState _s, const int64_t& _tick)
{
    return false;
}
