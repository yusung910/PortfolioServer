#pragma once
#include "SingletonClient.h"

class ObjectStatsManager : public SingletonClient<ObjectStatsManager>
{
private:

    //������ �⺻ ���� ���� <level, dic>
    std::unordered_map<int, MDBPilgrimLevelBaseStat*> m_umLevelBaseStatDic;

public:
    ObjectStatsManager();
    ~ObjectStatsManager();


    void SetLevelBaseStatDictionary(std::vector<MDBPilgrimLevelBaseStat*>* _statList);
    MDBPilgrimLevelBaseStat* GetLevelBaseStatDictionary(int _lvl);

};

