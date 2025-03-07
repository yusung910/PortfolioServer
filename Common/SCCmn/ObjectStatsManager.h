#pragma once
#include "SingletonClient.h"

class ObjectStatsManager : public SingletonClient<ObjectStatsManager>
{
private:
    //·¹º§º° ±âº» ½ºÅÈ »çÀü <level, dic>
    std::unordered_map<int, MDBPilgrimLevelBaseStat*> m_umLevelBaseStatDic;

    //ÈÆ·Ã ½ºÅÈ(Èû,¹Î,¸Ç,¼÷)¿¡ µû¸¥ ½ºÅÈ ºñÀ², <statNo, dic>
    std::unordered_map<int, MDBAbilityStatRatePerTrainingStat*> m_umAbiliyStatRatePerTrainingStat;

    //°¢¼º¿¡ µû¸¥ A°ª
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

