/**
 *  @file NetworkHost.hxx
 *  @author YS
 *  @date 2024-10-17
 *  @project SCore
 *
 *  IOCP 통신에 사용될 host 
 *  Network Host -> 네트워크/인터넷을 통해 다른 컴퓨터들과 쌍방향 통신이 가능한 '컴퓨터(단말기)'
 *  다른 '컴퓨터(단말기)'와 통신 할 수 있는 '컴퓨터(단말기)'의 정보와 관련 함수가 정의 되어 있는 클래스
 *  서버 컴퓨터의 정보가 담긴 클래스(?)
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

    //패킷 압축 관련 클래스
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

    // 통신의 목적지가 클라이언트인지 여부
    // true : 클라이언트
    // false : 서버
    bool m_bIsClientHost = false;

    // 소켓이 종료(close)된 원인 코드
    ESocketCloseType m_eLastSocketCloseType = ESocketCloseType::Reset;

    //Packet History 패킷이 통신되는 이력을 관리하는 변수 목록
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
     *  m_pPacketCompressor 객체 생성
     */
    NetworkHostPO();

    /*!
     *  Destructor.
     *  Reset() 함수 호출
     */
    virtual ~NetworkHostPO();

    //NetworkHost 내부 멤버 변수들 초기화
    void Reset();

    /*!
     *  m_lBaseTaskCount 값을 증가한다.
     *  
     */
    void BeginBaseTask();
    

    /*!
     *  End Base Task.
     */
    void EndBaseTask(bool _rslt, const ESocketCloseType& _type = ESocketCloseType::FailedToBaseTask);


    //EContextType 별 함수 목록
    bool Close(ESocketCloseType _e);
    //
};

