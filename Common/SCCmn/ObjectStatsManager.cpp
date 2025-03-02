#include "SCCmnPrivate.h"
#include "ObjectStatsManager.h"

ObjectStatsManager::ObjectStatsManager()
{
}

ObjectStatsManager::~ObjectStatsManager()
{
}

void ObjectStatsManager::SetLevelBaseStatDictionary(std::vector<MDBPilgrimLevelBaseStat*>* _statList)
{
}

MDBPilgrimLevelBaseStat* ObjectStatsManager::GetLevelBaseStatDictionary(int _lvl)
{
    auto itBaseStat = m_umLevelBaseStatDic.find(_lvl);

    if (itBaseStat == m_umLevelBaseStatDic.end())
        return nullptr;

    return itBaseStat->second;
}
