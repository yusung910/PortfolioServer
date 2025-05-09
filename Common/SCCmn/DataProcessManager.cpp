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

void DataProcessManager::SetObjectStatisticsTemplate(std::unordered_map<int, MDBObjectStatistics*>* _heroStatMap)
{
    m_umTotalObjectStatisticsTemplateMap = _heroStatMap;
}

void DataProcessManager::_Clear()
{
    m_umTotalObjectStatisticsTemplateMap = nullptr;
}

void DataProcessManager::Reset()
{
    _Clear();
}
