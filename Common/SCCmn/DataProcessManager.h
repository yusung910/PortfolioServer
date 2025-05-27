#pragma once
#include "SingletonClient.h"
#include "Protocol_generated.h"

#include <array>

class DataProcessManager : public SingletonClient<DataProcessManager>
{

private: //변수 선언부


public: //함수 선언부
    DataProcessManager();
    ~DataProcessManager();

private:
    void _Clear();

public:
    void Reset();
};

