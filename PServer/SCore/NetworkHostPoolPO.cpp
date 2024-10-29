#include "stdafx.hxx"
#include "NetworkHostPoolPO.hxx"

#include "NetworkHostPO.hxx"

NetworkHostPoolPO::NetworkHostPoolPO()
{
}

NetworkHostPoolPO::~NetworkHostPoolPO()
{
    AutoLock(m_xLock);

    for (auto& iter : m_oActiveMap)
        SafeDelete(iter.second);

    m_oActiveMap.clear();

    for (auto& host : m_oFreeQueue)
        SafeDelete(host);

    m_oFreeQueue.clear();
}

NetworkHostPO* NetworkHostPoolPO::Allocate()
{
    NetworkHostPO* localHost = nullptr;

    AutoLock(m_xLock);
    if (m_oFreeQueue.empty() == false)
    {
        localHost = m_oFreeQueue.front();
        m_oFreeQueue.pop_front();
    }
    else
    {
        localHost = new NetworkHostPO;
    }

    while (true)
    {
        ++m_nLastHostID;

        //m_nLastHostID의 값이 20억이 넘을 경우 1로 순회
        if (m_nLastHostID > 2000000000)
            m_nLastHostID = 1;

        //맵에 있을 경우 pass
        if (m_oActiveMap.find(m_nLastHostID) != m_oActiveMap.end())
            continue;

        localHost->SetHostID(m_nLastHostID);
        m_oActiveMap[m_nLastHostID] = localHost;

        break;
    }

    return localHost;
}

void NetworkHostPoolPO::Release(NetworkHostPO* _host)
{
    if (_host == nullptr)
        return;

    AutoLock(m_xLock);
    m_oActiveMap.erase(_host->GetHostID());
    _host->Reset();
    m_oFreeQueue.push_back(_host);
}

bool NetworkHostPoolPO::Check(const int& _hostID)
{
    AutoLock(m_xLock);
    return m_oActiveMap.find(_hostID) != m_oActiveMap.end();
}

NetworkHostPO* NetworkHostPoolPO::GetHost(const int& _hostID)
{
    AutoLock(m_xLock);
    if (auto it = m_oActiveMap.find(_hostID); it != m_oActiveMap.end())
        return it->second;

    return nullptr;
}
