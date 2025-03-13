#include "PGPPrivate.h"
#include "NPCStatistics.h"
#include "NPC.h"
NPCStatistics::NPCStatistics(NPC& _npc)
    :m_oNPC(_npc)
{
    
}

void NPCStatistics::Reset()
{
    m_oMDBNPCData = nullptr;
}

bool NPCStatistics::SetMDBData(const MDBNPC* _data)
{
    m_oMDBNPCData = _data;

    m_oNPC.SetNPCUniqueID(_data->NPCUniqueID);
    m_oNPC.SetObjectStats(_data->ObjectStat);
    return false;
}


