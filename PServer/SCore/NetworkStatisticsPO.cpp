#include "stdafx.hxx"
#include "NetworkStatisticsPO.hxx"

void NetworkStatisticsPO::Send(const int& _size)
{
    m_nTotalSendCount++;
    m_nTotalSendBytes += _size;
}

void NetworkStatisticsPO::Recv(const int& _size)
{
    m_nTotalRecvCount++;
    m_nTotalRecvBytes += _size;
}

void NetworkStatisticsPO::Connect()
{
    m_nCurrentConnectionCount++;
    m_nTotalConnectionCount++;
}

void NetworkStatisticsPO::Disconnect()
{
    m_nCurrentConnectionCount--;
}

void NetworkStatisticsPO::Reset()
{
    // Send & Receive 트래픽
    m_nLastCheckRecvBytes = 0;
    m_nLastCheckSendBytes = 0;

    m_nTotalRecvBytes = 0;
    m_nTotalSendBytes = 0;

    // Send & Recevie 패킷 수
    m_nLastCheckRecvCount = 0;
    m_nLastCheckSendCount = 0;

    m_nTotalRecvCount = 0;
    m_nTotalSendCount = 0;

    // Connection 수
    m_nCurrentConnectionCount = 0;
    m_nTotalConnectionCount = 0;
}

int64_t NetworkStatisticsPO::GetCurrentSendBytes()
{
    int64_t lRet = m_nTotalSendBytes - m_nLastCheckSendBytes;
    m_nLastCheckSendBytes.exchange(m_nTotalSendBytes);
    return lRet;
}

int64_t NetworkStatisticsPO::GetCurrentRecvBytes()
{
    int64_t lRet = m_nTotalRecvBytes - m_nLastCheckRecvBytes;
    m_nLastCheckRecvBytes.exchange(m_nTotalRecvBytes);
    return lRet;
}

int64_t NetworkStatisticsPO::GetCurrentSendCount()
{
    int64_t lRet = m_nTotalSendCount - m_nLastCheckSendCount;
    m_nLastCheckSendCount.exchange(m_nTotalSendCount);
    return lRet;
}

int64_t NetworkStatisticsPO::GetCurrentRecvCount()
{
    int64_t lRet = m_nTotalRecvCount - m_nLastCheckRecvCount;
    m_nLastCheckRecvCount.exchange(m_nTotalRecvCount);
    return lRet;
}

int NetworkStatisticsPO::GetCurrentConnection()
{
    return m_nCurrentConnectionCount;
}

int64_t NetworkStatisticsPO::GetTotalSendBytes() const
{
    return m_nTotalSendBytes;
}

int64_t NetworkStatisticsPO::GetTotalRecvBytes() const
{
    return m_nTotalRecvBytes;
}

int64_t NetworkStatisticsPO::GetTotalSendCount() const
{
    return m_nTotalSendCount;
}

int64_t NetworkStatisticsPO::GetTotalRecvCount() const
{
    return m_nTotalRecvCount;
}

int NetworkStatisticsPO::GetTotalConnection() const
{
    return m_nTotalConnectionCount;
}



