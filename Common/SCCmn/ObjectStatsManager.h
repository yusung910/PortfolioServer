#pragma once
#include "SingletonClient.h"

class ObjectStatsManager : public SingletonClient<ObjectStatsManager>
{
private:
    //������ �⺻ ���� ���� <level, dic>
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umRenownAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umStrAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umDexAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umEndureAbilityDic;
    std::unordered_map<int, MDBPilgrimLevelAbility*> m_umWeaponMasteryAbilityDic;

    //������ ���� A��
    std::unordered_map<int, int> m_umAwakenAValues;



public:
    ObjectStatsManager();
    ~ObjectStatsManager();


    void SetLevelAbilityDictionary(std::vector<MDBPilgrimLevelAbility*>* _statList);
    MDBPilgrimLevelAbility* GetLevelBaseAbility(EPilgrimTrainingAbility::Type _type, int _lvl);


    void SetAwakenAValue(std::vector< MDBAwakenAValue*>* _aValueList);
    int GetAValue(int _awakeSeq);



};

