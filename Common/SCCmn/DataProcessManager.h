#pragma once
#include "SingletonClient.h"
#include "Protocol_generated.h"

#include <array>

class DataProcessManager : public SingletonClient<DataProcessManager>
{

private: //변수 선언부

    //HeroStat
    std::unordered_map<int, MDBObjectStatistics*>* m_umTotalObjectStatisticsTemplateMap;


public: //함수 선언부
    DataProcessManager();
    ~DataProcessManager();

    void SetObjectStatisticsTemplate(std::unordered_map<int, MDBObjectStatistics*>* _pilgrimStatMap);

private:
    void _Clear();

public:
    void Reset();
};

