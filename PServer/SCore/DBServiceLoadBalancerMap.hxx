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

    bool Push(const int& _serverID, InnerPacket::SharedPtr _data);
    bool AddService(const int& _serverID, DBService* _service);
    bool IsExistServerID(const int& _serverID);
    std::unordered_map<int, DBService*>& GetDBServiceList();

    void Exit();
};

