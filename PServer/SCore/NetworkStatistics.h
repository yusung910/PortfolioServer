/**
 *  @file NetworkStatistics.h
 *  @author YS
 *  @date 2024-10-24
 *  @project SCore
 *
 *  네트워크 전송, 연결 상태 등을 관리하기 위한 클래스
 */
#pragma once
#include "SCoreAPI.h"

class NetworkStatisticsPO;

class SCoreAPI NetworkStatistics
{
private:
    NetworkStatisticsPO* m_pPO = nullptr;

public:
    NetworkStatistics();
    ~NetworkStatistics();

    void Reset();

    void Send(const int& _size);
    void Recv(const int& _size);
    void Connect();
    void Disconnect();

    int64_t SendBytes();
    int64_t RecvBytes();
    int64_t SendCount();
    int64_t RecvCount();
    int CurrentConnection();

    int64_t TotalSendBytes() const;
    int64_t TotalRecvBytes() const;
    int64_t TotalSendCount() const;
    int64_t TotalRecvCount() const;
    int TotalConnection() const;
};

