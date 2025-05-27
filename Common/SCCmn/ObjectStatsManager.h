#pragma once
#include "SingletonClient.h"

class ObjectStatsManager : public SingletonClient<ObjectStatsManager>
{
private:
    //레벨별 기본 스탯 사전 <level, dic>
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umRenownAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umStrAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umDexAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umEndureAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umWeaponMasteryAbilityDic;

    //각성에 따른 A값
    std::unordered_map<int, int> m_umAwakenAValues;



public:
    ObjectStatsManager();
    ~ObjectStatsManager();


    void SetLevelAbilityDictionary(std::vector<MDBPilgrimLevelAbility*>* _statList);
    MDBPilgrimLevelAbility* GetLevelBaseAbility(EPilgrimTrainingAbility::Type _type, int _lvl);


    void SetAwakenAValue(std::vector< MDBAwakenAValue*>* _aValueList);
    int GetAValue(int _awakeSeq);



};

