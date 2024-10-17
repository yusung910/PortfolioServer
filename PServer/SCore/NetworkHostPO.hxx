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

constexpr size_t MAX_MESSAGE_ID_HISTORY_SIZE = 100;

class NetworkHostPO
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

    //Packet History ��Ŷ�� ��ŵǴ� �̷��� �����ϴ� ���� ���
    const int64_t m_nPACKET_RECV_CHECK_TICK = 10000;
    const int64_t m_fPACKET_RECV_CHECK_COUNT_PER_SEC = 30.f;

    bool            m_bUsePacketRecvCheck = false;
    int64_t         m_nPacketRecvCheckTick = 0;
    std::atomic_int m_nPacketRecvCheckCountter = 0;
    
    std::atomic_int m_nMessageHistoryIdx = 0;
    std::array<std::tuple<int, int64_t>, MAX_MESSAGE_ID_HISTORY_SIZE> m_oMessageHistory = {};

    std::atomic_int64_t m_nLastPacketTick = 0;
    // Packet History End

public:
    /*!
     *  Constructor.
     *  m_pPacketCompressor ��ü ����
     */
    NetworkHostPO();

    /*!
     *  Destructor.
     *  Reset() �Լ� ȣ��
     */
    virtual ~NetworkHostPO();

    //NetworkHost ���� ��� ������ �ʱ�ȭ
    void Reset();

    /*!
     *  m_lBaseTaskCount ���� �����Ѵ�.
     *  
     */
    void BeginBaseTask();
    

    /*!
     *  End Base Task.
     */
    void EndBaseTask(bool _rslt, const ESocketCloseType& _type = ESocketCloseType::FailedToBaseTask);


    //EContextType �� �Լ� ���
    bool Close(ESocketCloseType _e);
    //
};

