#include "stdafx.hxx"
#include "NetworkHostPoolPO.hxx"

#include "NetworkHostPO.hxx"

NetworkHostPoolPO::NetworkHostPoolPO()
{
}

NetworkHostPoolPO::~NetworkHostPoolPO()
{
    AutoLock(m_xLock);

    for (auto& iter : m_umActiveMap)
        SafeDelete(iter.second);

    m_umActiveMap.clear();

    for (auto& host : m_oFreeQueue)
        SafeDelete(host);

    m_oFreeQueue.clear();
}

NetworkHostPO* NetworkHostPoolPO::Allocate()
{
    NetworkHostPO* lHost = nullptr;

    AutoLock(m_xLock);
    if (m_oFreeQueue.empty() == false)
    {
        lHost = m_oFreeQueue.front();
        m_oFreeQueue.pop_front();
    }
    else
    {
        lHost = new NetworkHostPO;
    }

    while (true)
    {
        ++m_nLastHostID;

        //m_nLastHostID의 값이 20억이 넘을 경우 1로 순회
        if (m_nLastHostID > 2000000000)
            m_nLastHostID = 1;

        //맵에 있을 경우 pass
        if (m_umActiveMap.find(m_nLastHostID) != m_umActiveMap.end())
            continue;

        lHost->SetHostID(m_nLastHostID);
        m_umActiveMap[m_nLastHostID] = lHost;

        break;
    }

    return lHost;
}

void NetworkHostPoolPO::Release(NetworkHostPO* _host)
{
    if (_host == nullptr)
        return;

    AutoLock(m_xLock);
    m_umActiveMap.erase(_host->GetHostID());
    _host->Reset();
    m_oFreeQueue.push_back(_host);
}

bool NetworkHostPoolPO::Check(const int& _hostID)
{
    AutoLock(m_xLock);
    return m_umActiveMap.find(_hostID) != m_umActiveMap.end();
}

NetworkHostPO* NetworkHostPoolPO::GetHost(const int& _hostID)
{
    AutoLock(m_xLock);
    if (auto it = m_umActiveMap.find(_hostID); it != m_umActiveMap.end())
        return it->second;

    return nullptr;
}
