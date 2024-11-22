/**
 *  @file NetworkStatisticsPO.hxx
 *  @author YS
 *  @date 2024-10-21
 *  @project SCore
 *
 *  네트워크 통신 통계를 관리하기 위한 클래스
 */
#pragma once
#include "Singleton.h"

#include <atomic>
#include <chrono>

class NetworkStatisticsPO
{
private:
    // Send & Receive 트래픽
    std::atomic_int64_t m_nLastCheckRecvBytes = 0;
    std::atomic_int64_t m_nLastCheckSendBytes = 0;

    std::atomic_int64_t m_nTotalRecvBytes = 0;
    std::atomic_int64_t m_nTotalSendBytes = 0;

    // Send & Recevie 패킷 수
    std::atomic_int64_t m_nLastCheckRecvCount = 0;
    std::atomic_int64_t m_nLastCheckSendCount = 0;

    std::atomic_int64_t m_nTotalRecvCount = 0;
    std::atomic_int64_t m_nTotalSendCount = 0;

    // Connection 수
    std::atomic_int m_nCurrentConnectionCount = 0;
    std::atomic_int m_nTotalConnectionCount = 0;

public:
    NetworkStatisticsPO() = default;
    ~NetworkStatisticsPO() = default;

    void Send(const int& _size);
    void Recv(const int& _size);
    void Connect();
    void Disconnect();

    void Reset();

    int64_t GetCurrentSendBytes();
    int64_t GetCurrentRecvBytes();
    int64_t GetCurrentSendCount();
    int64_t GetCurrentRecvCount();
    int GetCurrentConnection();

    int64_t GetTotalSendBytes() const;
    int64_t GetTotalRecvBytes() const;
    int64_t GetTotalSendCount() const;
    int64_t GetTotalRecvCount() const;
    int GetTotalConnection() const;


};

