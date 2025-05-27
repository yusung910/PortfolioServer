#pragma once

struct MDBNPCSpawn;

struct SpawnInfo
{
    int m_nNPCUniqueID = 0;
    int m_nNPCSpawnCount = 0;
    int m_nRespawnTime = 0;
    int m_nSpawnGroupSeq = 0;
    const MDBNPCSpawn* m_oSpawn = nullptr;
};