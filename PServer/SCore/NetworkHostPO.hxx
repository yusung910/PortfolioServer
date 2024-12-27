/**
 *  @file NetworkHost.hxx
 *  @author YS
 *  @date 2024-10-17
 *  @project SCore
 *
 *  IOCP 통신에 사용될 host
 *  Network Host -> 네트워크/인터넷을 통해 다른 컴퓨터들과 쌍방향 통신이 가능한 '컴퓨터(단말기)'
 *  다른 '컴퓨터(단말기)'와 통신 할 수 있는 '컴퓨터(단말기)'의 정보와 관련 함수가 정의 되어 있는 클래스
 *  서버 컴퓨터의 정보가 담긴 클래스
 *  https://lxxyeon.tistory.com/172
 *
 *  NetworkContext의 Type별로 함수들이 존재한다(예외 Waiting)
 */
#pragma once
#include "BlockingQueue.h"
#include "NetworkCommon.h"
#include "PacketCompressor.hxx"
#include <array>

constexpr int ACCEPT_WAIT_COUNT = 100;

//Forward Declarations
class NetworkEventSync;
class NetworkContextPO;

constexpr size_t MAX_MESSAGE_ID_HISTORY_SIZE = 100;

class NetworkHostPO
{
private:

    //패킷 압축 관련 클래스
    PacketCompressor::SharedPtr m_pPacketCompressor = nullptr;

    int m_nHostID = 0;
    SOCKET m_oSocket = INVALID_SOCKET;

    //Host 유형
    EHostType m_eHostType = EHostType::None;

    // 네트워크 통신 event 싱크를 맞추기 위한 클래스를 Pacade 형태로 이용
    // 다른 서버에서 사용 할 때 NetworkEventSync클래스를 상속 받은 클래스를 생성하고
    // 각각의 함수를 재정의 한 후 SetEventSync()함수로 지정해서 사용해야한다
    NetworkEventSync* m_pEventSync = nullptr;

    //기초 작업 수
    volatile long m_lBaseTaskCount = 0;
    //전송 작업 수
    volatile long m_lSendTaskCount = 0;

    int64_t m_nCheckTimeoutMS = 0;
    int64_t m_nCheckAliveMS = 0;

    std::string m_sIP = "";

    int m_nIP = 0;
    int m_nPort = 0;

    std::mutex m_xSendLock;

    //패킷 전송 대기 목록
    std::deque<Packet::SharedPtr> m_oSendWaitingList;

    //전송 패킷 큐
    std::deque<Packet::SharedPtr> m_oSendWorkQueue;

    // 통신의 목적지가 클라이언트인지 여부
    // true : 클라이언트
    // false : 서버
    bool m_bIsClientHost = false;

    // 소켓이 종료(close)된 원인 코드
    ESocketCloseType m_eLastSocketCloseType = ESocketCloseType::Reset;

    //Packet History 패킷이 통신되는 이력을 관리하는 변수 목록
    const int64_t m_nPACKET_RECV_CHECK_TICK = 10000;
    const float   m_fPACKET_RECV_CHECK_COUNT_PER_SEC = 30.f;

    bool            m_bUsePacketRecvCheck = false;
    int64_t         m_nPacketRecvCheckTick = 0;
    std::atomic_int m_nPacketRecvCheckCounter = 0;

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
     */
    void BeginBaseTask();


    /*!
     *  기본 task 종료처리.
     */
    void EndBaseTask(bool _rslt, const ESocketCloseType& _type = ESocketCloseType::FailedToBaseTask);

    /*!
     *  Begins the send task.
     */
    void BeginSendTask();

    /*!
     *  네트워크 호스트 task 전송 종료처리
     *
     *      @param [in] _rslt If true, rslt. Otherwise not rslt.
     */
    void EndSendTask(bool _rslt);

    //EContextType 별 함수 목록
    //enum class EContextType : int
    //{
    //    None = 0,
    //    Listen,
    //    Join,
    //    Accept,
    //    Connect,
    //    Receive,
    //    Send,
    //    Encrypt,
    //    Close,
    //};

    /*!
     *  NetworkContext를 참조형 변수로 전달 받아
     *  소켓 통신을 하기 위해 WSAIoctl()로 Connect한다
     *  NetworkContextPO의 참조 수 (ReferenceCount)를 증가한다
     *
     *  NetworkContext로 소켓을 통신 할 수 있는 상태로 만든다
     *      @param [in,out] NetworkContextPO& _ctxt
     *
     *      @return
     */
    bool Connect(NetworkContextPO& _ctxt);

    /*!
     *  소켓의 상태를 listen()로 수신 대기 상태로 지정한다
     *
     *      @return
     */
    bool Listen();

    /*!
     *  클라이언트와의 연결을 수락하고 로컬주소와 원격 주소를 반환한다
     *  AcceptEx(), GetAcceptExSockaddrs() 이용
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Accept(NetworkContextPO& _ctxt);

    /*!
     *  소켓에 접속한 클라이언트에 WSARecv()로 NetworkContext 데이터를 수신한다
     *  데이터를 수신 결과를 NetworkContextPO _ctxt에 기록한다
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Receive(NetworkContextPO& _ctxt);


    /*!
     *  NetworkContext에 저장된 데이터를 복호화한다
     *  복호화 후 Receive(NetworkContextPO& _ctxt)를 실행한다
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Decrypt(NetworkContextPO& _ctxt);

    /*!
     *  전송 대기 목록(m_oSendWaitingList)에 패킷을 추가하는 함수
     *
     *      @param [in] _packt
     *
     *      @return
     */
    bool Waiting(Packet::SharedPtr _packt);

    /*!
     *  패킷 암호화
     *  m_oSendWaitingList에 있는 전송 대기 패킷 목록을
     *  m_oSendWorkQueue에 이동(swap)하고
     *  NetworkContext에 기록(write)한다
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Encrypt(NetworkContextPO& _ctxt);

    /*!
     *  NetworkContext에 저장된 데이터를 Socket 통신으로 WSASend()로 송신한다
     *
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Send(NetworkContextPO& _ctxt);

    /*!
     *  NetworkHostPO에 연결된 Socket을 닫는다.
     *
     *      @param [in] _e
     *
     *      @return
     */
    bool Close(ESocketCloseType _e);
    //

    /*!
     *  서버 통신 가능 상태 확인
     *
     *      @return True if alive. False if not.
     */
    bool IsAlive();

    /*!
     *  NetworkController에서 _UpdateHost()함수를 통해 실행된다
     *  NetworkController에서 Timer로 등록되어 스레드(while 문)을 통해
     *  지속적으로 실행된다.
     *
     *  NetworkHostPO의 EHostType에 따라 아래의 함수들을 실행한다
     *  EHostType::Listener  -> UpdateListener(_appTimeMS)
     *  EHostType::Acceptor  -> UpdateAccepter(_appTimeMS)
     *  EHostType::Connector -> UpdateConnector(_appTimeMS)
     *
     *      @param [in] _appTimeMS
     */
    void Update(int64_t _appTimeMS);


    /*!
     *  NetworkHostPO의 EHostType이 Listener일 때 실행되는 함수.
     *  NetworkContextPO를 할당 받고 BeginBaseTask()를 실행,
     *  m_lBaseTaskCount값을 증가한다
     *
     *  할당받은 NetworkContextPO를 인자값으로 Accept() 함수를 실행한다.
     *  Accept() 함수 결과 값이 false일 경우 EndBaseTask(true)로 실행한다.
     *
     *  실행 후 NetworkManager로 NetworkContext를 Release하기 위한 함수
     *  ReleaseContext()를 실행한다.
     *
     *      @param [in] _appTimeMS
     */
    void UpdateListener(int64_t _appTimeMS);


    /*!
     *  NetworkHostPO의 EHostType이 Acceptor일 때 실행되는 함수.
     *  m_bUsePacketRecvCheck 값이 true일 때 패킷 전송량에 따른 시간을 체크한다
     *  전송 시간이 m_fPACKET_RECV_CHECK_COUNT_PER_SEC(30.f)이상 될 경우
     *  소켓을 종료한다.
     *
     *  패킷 통신 처리하기 위한 BeginSendTask()를 실행한다.
     *
     *      @param [in] _appTimeMS
     */
    void UpdateAccepter(int64_t _appTimeMS);


    /*!
     *  NetworkHostPO의 EHostType이 Connector일 때 실행되는 함수.
     *  m_nCheckAliveMS 값(tick + DEFAULT_NETWORK_ALIVE_MS)을 기록하고
     *  BeginSendTask() 함수를 실행한다
     *
     *      @param [in] _appTimeMS
     */
    void UpdateConnector(int64_t _appTimeMS);

    /*!
     *  NetworkContextPO의 EContextType이 Join일 때 EHostType 값이 Connector로 실행되고
     *  NetworkContextPO의 EContextType이 Connect일 때 EHostType 값이 Acceptor로 실행된다
     *  setsockopt() 함수로 optname이 IPPROTO_TCP의 소켓의 옵션을 TCP_NODELAY로 변경하고
     *  NetworkHostPO에 할당 된 m_pEventSync의 OnConnect함수를 실행한다
     *
     *  인자값으로 전달 받은 EHostType을 멤버변수 m_eHostType에 할당한다.
     *      @param [in] _type
     */
    void EventConnect(const EHostType& _type);


    /*!
     *  NetworkHost가 NetworkControllerPO에서 종료되었을 때 실행 되는 함수.
     *  NetworkControllerPO의 _UpdateHost(), _AddHost()에서 조건에 따라 실행 된다.
     */
    void EventClose();


    /*!
     *  수신 된 패킷 데이터를 복호화 했을 때 실행 되는 함수
     *  m_pEventSync의 OnReceive함수가 실행되며
     *  _AddReceive() 함수에 tick 데이터가 인자값으로 실행되어
     *  m_nLastPacketTick에 저장한다
     *
     *      @param [in]     _msgID
     *      @param [in,out] _msg
     *      @param [in]     _msgSize
     */
    void EventReceive(int _msgID, char* _msg, int _msgSize);

public:
    const int& GetHostID() const;
    void SetHostID(const int& _id);

    SOCKET GetSocket();
    void SetSocket(SOCKET _sock);

    const EHostType& GetHostType() const;
    void SetHostType(const EHostType& _type);

    NetworkEventSync* GetEvnetSync();
    void SetEventSync(NetworkEventSync* _eventSync);

    std::string& GetIP();
    void SetIP(std::string _ip);
    int GetIPInt32();
    void SetIPInt32(int _n);

    int64_t GetLastPacketTick();
    int GetPeerPort();
    void SetPeerPort(const int& _port);

    void SetClientHostMode(const bool& _onoff);



private:
    /*!
     *  ESocketCloseType에 따른 메세지를 반환한다
     *
     *      @param [in] _e
     *
     *      @return The socket close type string.
     */
    std::wstring _GetSocketCloseTypeString(const ESocketCloseType& _e);

    /*!
     *  NetworkHost 타입에 따른 메세지를 반환
     *
     *      @param [in] _type
     *
     *      @return The host type.
     */
    const wchar_t* _GetHostType(const EHostType& _type);

    /*!
     *  수신 받은 패킷의 MessageID 리스트를 String으로 만들어 출력한다.
     */
    void _GetRecvHistoryStackString();

    /*!
     *  수신한 network 패킷 이력들을 반환한다
     *
     *      @param [in,out] _list
     */
    void _GetRecvHistory(std::vector<std::tuple<int, int64_t>>& _list);

    /*!
     *  인자값으로 전달받은 패킷의 MessageID와
     *  Tick 데이터를 기록한다 (로그)
     *
     *      @param [in] _msgID
     *      @param [in] _tick
     */
    void _AddReceive(const int& _msgID, const int64_t& _tick);
};

