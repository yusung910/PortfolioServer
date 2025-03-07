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
    if (nullptr == _statList)
        return;

    m_umLevelBaseStatDic.clear();

    for (auto& tmp : *_statList)
    {
        if (nullptr == tmp)
            continue;

        if (tmp->Level > G_N_MAX_LEVEL)
            continue;

        auto itLevel = m_umLevelBaseStatDic.find(tmp->Level);
        if (itLevel != m_umLevelBaseStatDic.end())
        {
            continue;
        }

        m_umLevelBaseStatDic.emplace(tmp->Level, tmp);
    }

}

MDBPilgrimLevelBaseStat* ObjectStatsManager::GetLevelBaseStatDictionary(int _lvl)
{
    auto itBaseStat = m_umLevelBaseStatDic.find(_lvl);

    if (itBaseStat == m_umLevelBaseStatDic.end())
        return nullptr;

    return itBaseStat->second;
}

void ObjectStatsManager::SetAbilityStatRatePerTrainingStat(std::vector<MDBAbilityStatRatePerTrainingStat*>* _rateList)
{
    if (nullptr == _rateList)
        return;

    m_umAbiliyStatRatePerTrainingStat.clear();

    for (auto& tmp : *_rateList)
    {
        if (nullptr == tmp)
            continue;

        if (false == EAbility::IsValid((EAbility::Type)tmp->StatNo))
            continue;

        if (true == EAbility::IsTrainingStat((EAbility::Type)tmp->StatNo))
            continue;

        switch ((EAbility::Type)tmp->StatNo)
        {
        case EAbility::HP:
        case EAbility::MP:
            continue;

        default:
            m_umAbiliyStatRatePerTrainingStat.emplace(tmp->StatNo, tmp);
            break;
        }
    }

}

MDBAbilityStatRatePerTrainingStat* ObjectStatsManager::GetAbilityStatRatePerTrainingStat(int _statNo)
{
    auto it = m_umAbiliyStatRatePerTrainingStat.find(_statNo);

    if (it == m_umAbiliyStatRatePerTrainingStat.end())
        return nullptr;

    return it->second;
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
