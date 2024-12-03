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
