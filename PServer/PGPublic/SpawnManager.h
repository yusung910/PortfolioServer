#pragma once
#include "RefSingleton.h"
#include "MapLoadData.h"
#include <unordered_map>
#include <vector>


class SpawnData;
class SpawnManager
{
private:
    std::unordered_map<int, std::vector<SpawnData*>> m_umMapSpawnDataList;
    int m_nTotalNPCCnt = 0;

public:
    SpawnManager() = default;
    virtual ~SpawnManager();

    bool Spawn(SpawnData* _spawnData, int64_t _phase = 0, bool _npcCellCheck = true);

    int GetRandomSpawnNPCUniqueID(int _npcSpawnGroupID);
private:

};

