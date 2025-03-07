#pragma once
#include "SingletonClient.h"

class ObjectStatsManager : public SingletonClient<ObjectStatsManager>
{
private:
    //������ �⺻ ���� ���� <level, dic>
    std::unordered_map<int, MDBPilgrimLevelBaseStat*> m_umLevelBaseStatDic;

    //�Ʒ� ����(��,��,��,��)�� ���� ���� ����, <statNo, dic>
    std::unordered_map<int, MDBAbilityStatRatePerTrainingStat*> m_umAbiliyStatRatePerTrainingStat;

    //������ ���� A��
    std::unordered_map<int, int> m_umAwakenAValues;



public:
    ObjectStatsManager();
    ~ObjectStatsManager();


    void SetLevelBaseStatDictionary(std::vector<MDBPilgrimLevelBaseStat*>* _statList);
    MDBPilgrimLevelBaseStat* GetLevelBaseStatDictionary(int _lvl);

    void SetAbilityStatRatePerTrainingStat(std::vector< MDBAbilityStatRatePerTrainingStat*>* _rateList);
    MDBAbilityStatRatePerTrainingStat* GetAbilityStatRatePerTrainingStat(int _statNo);


    void SetAwakenAValue(std::vector< MDBAwakenAValue*>* _aValueList);
    int GetAValue(int _awakeSeq);



};

