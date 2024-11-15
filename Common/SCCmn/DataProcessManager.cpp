#include "SCCmnPrivate.h"
#include "DataProcessManager.h"

DataProcessManager::DataProcessManager()
{
    _Clear();
}

DataProcessManager::~DataProcessManager()
{
    _Clear();
}

void DataProcessManager::SetHeroStatTemplate(std::unordered_map<int, MDBHeroStat*>* _heroStatMap)
{
    m_umTotalHeroStatTemplateMap = _heroStatMap;
}

void DataProcessManager::_Clear()
{
    m_umTotalHeroStatTemplateMap = nullptr;
}

void DataProcessManager::Reset()
{
    _Clear();
}
