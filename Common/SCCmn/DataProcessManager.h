#pragma once
#include "SingletonClient.h"
#include <array>
#include "Protocol_generated.h"

class DataProcessManager : public SingletonClient<DataProcessManager>
{
    
private: //변수 선언부

    //HeroStat
    std::unordered_map<int, MDBHeroStat*>* m_umTotalHeroStatTemplateMap;

public: //함수 선언부
    DataProcessManager();
    ~DataProcessManager();

    void SetHeroStatTemplate(std::unordered_map<int, MDBHeroStat*>* _heroStatMap);

private:
    void _Clear();

public:
    void Reset();
};

