#include "stdafx.hxx"
#include "DBServiceLoadBalancerMap.hxx"
#include "DBService.h"

DBServiceLoadBalancerMap::DBServiceLoadBalancerMap()
{
}

DBServiceLoadBalancerMap::~DBServiceLoadBalancerMap()
{
    Exit();
}

bool DBServiceLoadBalancerMap::Push(const int& _serverID, InnerPacket::SharedPtr _data)
{
    auto it = m_umDBServiceList.find(_serverID);
    if (it == m_umDBServiceList.end())
        return false;

    if (nullptr == it->second)
        return false;

    return it->second->Push(_data);
}

bool DBServiceLoadBalancerMap::AddService(const int& _serverID, DBService* _service)
{
    if(nullptr == _service)
        return false;

    auto it = m_umDBServiceList.find(_serverID);

    if (it != m_umDBServiceList.end())
        return false;

    m_umDBServiceList.insert_or_assign(_serverID, _service);
    return true;
}

bool DBServiceLoadBalancerMap::IsExistServerID(const int& _serverID)
{
    return m_umDBServiceList.find(_serverID) != m_umDBServiceList.end();
}

std::unordered_map<int, DBService*>& DBServiceLoadBalancerMap::GetDBServiceList()
{
    return m_umDBServiceList;
}

void DBServiceLoadBalancerMap::Exit()
{
    for (auto it = m_umDBServiceList.begin(); it != m_umDBServiceList.end(); ++it)
    {
        if (nullptr != it->second)
            it->second->Exit();
        SafeDelete(it->second);
    }

    m_umDBServiceList.clear();
}
