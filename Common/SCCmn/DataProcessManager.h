#pragma once
#include "SingletonClient.h"
#include <array>
#include "Protocol_generated.h"

class DataProcessManager : public SingletonClient<DataProcessManager>
{
    
private: //���� �����

    //HeroStat
    std::unordered_map<int, MDBHeroStat*>* m_umTotalHeroStatTemplateMap;

public: //�Լ� �����
    DataProcessManager();
    ~DataProcessManager();

    void SetHeroStatTemplate(std::unordered_map<int, MDBHeroStat*>* _heroStatMap);

private:
    void _Clear();

public:
    void Reset();
};

