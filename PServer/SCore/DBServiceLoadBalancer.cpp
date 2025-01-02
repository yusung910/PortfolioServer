#include "stdafx.hxx"
#include "DBServiceLoadBalancer.h"
#include "DBServiceLoadBalancerMap.hxx"

DBServiceLoadBalancer::DBServiceLoadBalancer()
{
    m_pServerIDServerList = new DBServiceLoadBalancerMap();
}

DBServiceLoadBalancer::~DBServiceLoadBalancer()
{
    Exit();
    SafeDelete(m_pServerIDServerList);
    SafeDelete(m_pDirectService);
    SafeDeleteArray(m_pServiceList);

}

bool DBServiceLoadBalancer::Push(InnerPacket::SharedPtr _data)
{
    if (false == m_bInitialized)
        return false;

    if (nullptr == _data.get())
        return false;

    if (m_nServiceCount <= 0)
        return false;

    if (nullptr == m_pServiceList)
        return false;

    size_t lIdx = abs(_data->m_nWorkerID) & m_nServiceCount;

    if (nullptr == m_pServiceList[lIdx])
        return false;

    return m_pServiceList[lIdx]->Push(_data);
}

bool DBServiceLoadBalancer::PushByDBID(InnerPacket::SharedPtr _data, const int& _serverID)
{
    if (_serverID == 0)
        return Push(_data);

    if (false == m_bInitialized)
        return false;

    if (nullptr == _data.get())
        return false;

    if (nullptr == m_pServerIDServerList)
        return false;

    return m_pServerIDServerList->Push(_serverID, _data);
}

bool DBServiceLoadBalancer::PushDirect(InnerPacket::SharedPtr _data)
{
    if (false == m_bInitialized)
        return false;

    if (nullptr == _data.get())
        return false;

    if (nullptr == m_pDirectService)
        return false;

    return m_pDirectService->Push(_data);
}

void DBServiceLoadBalancer::Exit()
{
    if (nullptr != m_pDirectService)
    {
        m_pDirectService->Exit();
    }

    if (nullptr != m_pServiceList)
    {
        for (size_t i = 0; i < m_nServiceCount; i++)
        {
            if (nullptr != m_pServiceList[i])
            {
                m_pServiceList[i]->Exit();
                SafeDelete(m_pServiceList[i]);
            }
        }
    }

    if (nullptr != m_pServerIDServerList)
    {
        m_pServerIDServerList->Exit();
    }
}

bool DBServiceLoadBalancer::_AddDBService(const int& _serverID, DBService* _service)
{
    if (nullptr == m_pServerIDServerList)
        return false;
    return m_pServerIDServerList->AddService(_serverID, _service);
}

bool DBServiceLoadBalancer::_IsExistsServerID(const int& _serverID)
{
    if (nullptr == m_pServerIDServerList)
        return false;
    return m_pServerIDServerList->IsExistServerID(_serverID);
}
