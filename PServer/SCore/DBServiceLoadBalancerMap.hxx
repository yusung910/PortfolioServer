#pragma once
#include <unordered_map>
#include "InnerPacket.h"

#pragma warning(push)
#pragma warning(disable:4251)

class DBService;
class SCoreAPI DBServiceLoadBalancerMap
{
private:
    std::unordered_map<int, DBService*> m_umDBServiceList;

public:
    DBServiceLoadBalancerMap();
    ~DBServiceLoadBalancerMap();


    void Exit();
};

