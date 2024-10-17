/**
 *  @file NetworkHost.hxx
 *  @author YS
 *  @date 2024-10-17
 *  @project SCore
 *
 *  IOCP ��ſ� ���� host 
 *  Network Host -> ��Ʈ��ũ/���ͳ��� ���� �ٸ� ��ǻ�͵�� �ֹ��� ����� ������ '��ǻ��(�ܸ���)'
 *  �ٸ� '��ǻ��(�ܸ���)'�� ��� �� �� �ִ� '��ǻ��(�ܸ���)'�� ������ ���� �Լ��� ���� �Ǿ� �ִ� Ŭ����
 *  ���� ��ǻ���� ������ ��� Ŭ����(?)
 *  https://lxxyeon.tistory.com/172
 */
#pragma once
#include "BlockingQueue.h"
#include "NetworkCommon.h"
#include "PacketCompressor.hxx"
#include <array>

constexpr int ACCEPT_WAIT_COUNT = 100;

//Forward Declarations
class NetworkEventSync;
class NetworkContext;

class NetworkHost
{
private:

    //��Ŷ ���� ���� Ŭ����
    PacketCompressor::SharedPtr m_pPacketCompressor = nullptr;

    int m_nHostID = 0;
    SOCKET m_oSocket = INVALID_SOCKET;
    
    EHostType m_eHostType = EHostType::None;

    NetworkEventSync* m_pEventSync = nullptr;

    volatile long m_lBaseTaskCount = 0;
    volatile long m_lSendTaskCount = 0;

    int64_t m_nCheckTimeoutMS = 0;
    int64_t m_nCheckAliveMS = 0;

    std::string m_sIP = "";

    int m_nIP = 0;
    int m_nPort = 0;

    std::mutex m_oSendLock;

    //
    std::deque<Packet::SharedPtr> m_oSendWaitingList;

    //
    std::deque<Packet::SharedPtr> m_oSendWorkQueue;

    // ����� �������� Ŭ���̾�Ʈ���� ����
    // true : Ŭ���̾�Ʈ
    // false : ����
    bool m_bIsClientHost = false;

    // ������ ����(close)�� ���� �ڵ�
    ESocketCloseType m_eLastSocketCloseType = ESocketCloseType::Reset;

    //Packet History
    const int64_t m_nPacketReceiveCheckTick = 10000;

};

