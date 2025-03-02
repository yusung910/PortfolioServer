#pragma once
#include "SingletonClient.h"

class ObjectStatsManager : public SingletonClient<ObjectStatsManager>
{
private:

    //레벨별 기본 스탯 사전 <level, dic>
    std::unordered_map<int, MDBPilgrimLevelBaseStat*> m_umLevelBaseStatDic;

public:
    ObjectStatsManager();
    ~ObjectStatsManager();


    void SetLevelBaseStatDictionary(std::vector<MDBPilgrimLevelBaseStat*>* _statList);
    MDBPilgrimLevelBaseStat* GetLevelBaseStatDictionary(int _lvl);

};

