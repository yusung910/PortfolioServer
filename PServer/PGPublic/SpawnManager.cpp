#include "PGPPrivate.h"
#include "SpawnManager.h"
#include "SpawnData.h"

#include "MapDataManager.h"
#include "MDBDataManager.h"
#include <Clock.h>

SpawnManager::~SpawnManager()
{
}

bool SpawnManager::Spawn(SpawnData* _spawnData, int64_t _phase, bool _npcCellCheck)
{
    if (nullptr == _spawnData)
        return  false;

    for (auto& localInfo : _spawnData->GetSpawnInfoList())
    {
        if (localInfo.m_nNPCSpawnCount <= 0)
            continue;

        if (localInfo.m_oSpawn == nullptr)
            continue;

        int localMaxChannelCount = 1;

        if (nullptr != localInfo.m_oSpawn)
            localMaxChannelCount = MapDataManager::GetInst().GetChannelCount(_spawnData->GetMapID());

        for (int channel = 0; channel < localMaxChannelCount; channel++)
        {
            for (int i = 0; i < localInfo.m_nNPCSpawnCount; i++)
            {
                int localNPCUniqueID = 0;
                if (localInfo.m_nSpawnGroupSeq > 0)
                {
                    //MDBDataManager::GetInst().Get
                }
            }
        }
    }
    return false;
}

int SpawnManager::GetRandomSpawnNPCUniqueID(int _npcSpawnGroupID)
{
    //const auto& localNpcSpawnGroupList = MDBDataManager::GetInst().
    return 0;
}
