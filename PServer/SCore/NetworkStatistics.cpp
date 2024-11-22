#include "stdafx.hxx"
#include "NetworkStatistics.h"
#include "NetworkStatisticsPO.hxx"

NetworkStatistics::NetworkStatistics()
{
    m_pPO = new NetworkStatisticsPO();
}

NetworkStatistics::~NetworkStatistics()
{
    SafeDelete(m_pPO);
}

void NetworkStatistics::Reset()
{
    if (nullptr != m_pPO)
        m_pPO->Reset();
}

void NetworkStatistics::Send(const int& _size)
{
    if (_size <= 0)
        return;

    if (nullptr != m_pPO)
        m_pPO->Send(_size);
}

void NetworkStatistics::Recv(const int& _size)
{
    if (_size <= 0)
        return;

    if (nullptr != m_pPO)
        m_pPO->Recv(_size);
}

void NetworkStatistics::Connect()
{
    if (nullptr != m_pPO)
        m_pPO->Connect();
}

void NetworkStatistics::Disconnect()
{
    if (nullptr != m_pPO)
        m_pPO->Disconnect();
}

int64_t NetworkStatistics::SendBytes()
{
    if (nullptr == m_pPO) return 0;

    return m_pPO->GetCurrentSendBytes();
}

int64_t NetworkStatistics::RecvBytes()
{
    if (nullptr == m_pPO) return 0;

    return m_pPO->GetCurrentRecvBytes();
}

int64_t NetworkStatistics::SendCount()
{
    if (nullptr == m_pPO) return 0;

    return m_pPO->GetCurrentSendCount();
}

int64_t NetworkStatistics::RecvCount()
{
    if (nullptr == m_pPO) return 0;
    return m_pPO->GetCurrentRecvCount();
}

int NetworkStatistics::CurrentConnection()
{
    if (nullptr == m_pPO) return 0;
    return m_pPO->GetCurrentConnection();
}

int64_t NetworkStatistics::TotalSendBytes() const
{
    if (nullptr == m_pPO) return 0;
    return m_pPO->GetTotalSendBytes();
}

int64_t NetworkStatistics::TotalRecvBytes() const
{
    if (nullptr == m_pPO) return 0;
    return m_pPO->GetTotalRecvBytes();
}

int64_t NetworkStatistics::TotalSendCount() const
{
    if (nullptr == m_pPO) return 0;
    return m_pPO->GetTotalSendCount();
}

int64_t NetworkStatistics::TotalRecvCount() const
{
    if (nullptr == m_pPO) return 0;
    return m_pPO->GetTotalRecvCount();
}

int NetworkStatistics::TotalConnection() const
{
    if (nullptr == m_pPO) return 0;
    return m_pPO->GetTotalConnection();
}
