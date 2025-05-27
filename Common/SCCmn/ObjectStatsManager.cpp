#include "SCCmnPrivate.h"
#include "ObjectStatsManager.h"

ObjectStatsManager::ObjectStatsManager()
{
}

ObjectStatsManager::~ObjectStatsManager()
{
}

void ObjectStatsManager::SetLevelAbilityDictionary(std::vector<MDBPilgrimLevelAbility*>* _statList)
{
    if (nullptr == _statList)
        return;

    auto localSetExpDic = [&](MDBPilgrimLevelAbility* _abilityInfo, std::unordered_map<int, MDBPilgrimLevelAbility*> _dic) -> void
        {
            auto itLevel = _dic.find(_abilityInfo->Level);
            if (itLevel == _dic.end())
            {
                _dic.emplace(_abilityInfo->Level, _abilityInfo);
            }
        };

    m_umRenownAbilityDic.clear();
    m_umStrAbilityDic.clear();
    m_umDexAbilityDic.clear();
    m_umEndureAbilityDic.clear();
    m_umWeaponMasteryAbilityDic.clear();

    for (auto& tmp : *_statList)
    {
        if (nullptr == tmp)
            continue;

        if (tmp->Level > G_N_MAX_LEVEL)
            continue;

        switch ((EPilgrimTrainingAbility::Type)tmp->TrainingAbilityType)
        {
        case EPilgrimTrainingAbility::Renown:
            localSetExpDic(tmp, m_umRenownAbilityDic);
            break;
        case EPilgrimTrainingAbility::Strength:
            localSetExpDic(tmp, m_umStrAbilityDic);
            break;
        break;
        case EPilgrimTrainingAbility::Dexterity:
            localSetExpDic(tmp, m_umDexAbilityDic);
            break;
        break;
        case EPilgrimTrainingAbility::Endurance:
            localSetExpDic(tmp, m_umEndureAbilityDic);
            break;
        break;
        case EPilgrimTrainingAbility::WeaponMastery:
            localSetExpDic(tmp, m_umWeaponMasteryAbilityDic);
            break;
        break;
        default:
            break;
        }
    }
}

MDBPilgrimLevelAbility* ObjectStatsManager::GetLevelBaseAbility(EPilgrimTrainingAbility::Type _type, int _lvl)
{
    auto localGetLevelAbility = [&](int _lvl, std::unordered_map<int, MDBPilgrimLevelAbility*> _dic) -> MDBPilgrimLevelAbility*
        {
            auto ability = _dic.find(_lvl);
            if(ability == _dic.end())
                return nullptr;

            return ability->second;
        };

    MDBPilgrimLevelAbility* localRetPtr = nullptr;

    switch (_type)
    {
    case EPilgrimTrainingAbility::Renown:
        localRetPtr = localGetLevelAbility(_lvl, m_umRenownAbilityDic);
        break;
    case EPilgrimTrainingAbility::Strength:
        localRetPtr = localGetLevelAbility(_lvl, m_umStrAbilityDic);
        break;
    case EPilgrimTrainingAbility::Dexterity:
        localRetPtr = localGetLevelAbility(_lvl, m_umDexAbilityDic);
        break;
    case EPilgrimTrainingAbility::Endurance:
        localRetPtr = localGetLevelAbility(_lvl, m_umEndureAbilityDic);
        break;
    case EPilgrimTrainingAbility::WeaponMastery:
        localRetPtr = localGetLevelAbility(_lvl, m_umWeaponMasteryAbilityDic);
        break;
    }

    return localRetPtr;
}

void ObjectStatsManager::SetAwakenAValue(std::vector<MDBAwakenAValue*>* _aValueList)
{
    if (nullptr == _aValueList)
        return;

    for (auto& tmp : *_aValueList)
    {
        m_umAwakenAValues.emplace(tmp->AwakenSeq, tmp->AValue);
    }
}

int ObjectStatsManager::GetAValue(int _awakeSeq)
{
    auto it = m_umAwakenAValues.find(_awakeSeq);

    if(it == m_umAwakenAValues.end())
        return 0;

    return it->second;
}
