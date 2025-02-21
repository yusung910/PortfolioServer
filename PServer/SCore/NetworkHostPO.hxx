/**
 *  @file NetworkHost.hxx
 *  @author YS
 *  @date 2024-10-17
 *  @project SCore
 *
 *  IOCP ��ſ� ���� host
 *  Network Host -> ��Ʈ��ũ/���ͳ��� ���� �ٸ� ��ǻ�͵�� �ֹ��� ����� ������ '��ǻ��(�ܸ���)'
 *  �ٸ� '��ǻ��(�ܸ���)'�� ��� �� �� �ִ� '��ǻ��(�ܸ���)'�� ������ ���� �Լ��� ���� �Ǿ� �ִ� Ŭ����
 *  ���� ��ǻ���� ������ ��� Ŭ����
 *  https://lxxyeon.tistory.com/172
 *
 *  NetworkContext�� Type���� �Լ����� �����Ѵ�(���� Waiting)
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

    //��Ŷ ���� ���� Ŭ����
    PacketCompressor::SharedPtr m_pPacketCompressor = nullptr;

    int m_nHostID = 0;
    SOCKET m_oSocket = INVALID_SOCKET;

    //Host ����
    EHostType m_eHostType = EHostType::None;

    // ��Ʈ��ũ ��� event ��ũ�� ���߱� ���� Ŭ������ Pacade ���·� �̿�
    // �ٸ� �������� ��� �� �� NetworkEventSyncŬ������ ��� ���� Ŭ������ �����ϰ�
    // ������ �Լ��� ������ �� �� SetEventSync()�Լ��� �����ؼ� ����ؾ��Ѵ�
    NetworkEventSync* m_pEventSync = nullptr;

    //���� �۾� ��
    volatile long m_lBaseTaskCount = 0;
    //���� �۾� ��
    volatile long m_lSendTaskCount = 0;

    int64_t m_nCheckTimeoutMS = 0;
    int64_t m_nCheckAliveMS = 0;

    std::string m_sIP = "";

    int m_nIP = 0;
    int m_nPort = 0;

    std::mutex m_xSendLock;

    //��Ŷ ���� ��� ���
    std::deque<Packet::SharedPtr> m_oSendWaitingList;

    //���� ��Ŷ ť
    std::deque<Packet::SharedPtr> m_oSendWorkQueue;

    // ����� �������� Ŭ���̾�Ʈ���� ����
    // true : Ŭ���̾�Ʈ
    // false : ����
    bool m_bIsClientHost = false;

    // ������ ����(close)�� ���� �ڵ�
    ESocketCloseType m_eLastSocketCloseType = ESocketCloseType::Reset;

    //Packet History ��Ŷ�� ��ŵǴ� �̷��� �����ϴ� ���� ���
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
     */
    void BeginBaseTask();


    /*!
     *  �⺻ task ����ó��.
     */
    void EndBaseTask(bool _rslt, const ESocketCloseType& _type = ESocketCloseType::FailedToBaseTask);

    /*!
     *  Begins the send task.
     */
    void BeginSendTask();

    /*!
     *  ��Ʈ��ũ ȣ��Ʈ task ���� ����ó��
     *
     *      @param [in] _rslt If true, rslt. Otherwise not rslt.
     */
    void EndSendTask(bool _rslt);

    //EContextType �� �Լ� ���
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
     *  NetworkContext�� ������ ������ ���� �޾�
     *  ���� ����� �ϱ� ���� WSAIoctl()�� Connect�Ѵ�
     *  NetworkContextPO�� ���� �� (ReferenceCount)�� �����Ѵ�
     *
     *  NetworkContext�� ������ ��� �� �� �ִ� ���·� �����
     *      @param [in,out] NetworkContextPO& _ctxt
     *
     *      @return
     */
    bool Connect(NetworkContextPO& _ctxt);

    /*!
     *  ������ ���¸� listen()�� ���� ��� ���·� �����Ѵ�
     *
     *      @return
     */
    bool Listen();

    /*!
     *  Ŭ���̾�Ʈ���� ������ �����ϰ� �����ּҿ� ���� �ּҸ� ��ȯ�Ѵ�
     *  AcceptEx(), GetAcceptExSockaddrs() �̿�
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Accept(NetworkContextPO& _ctxt);

    /*!
     *  ���Ͽ� ������ Ŭ���̾�Ʈ�� WSARecv()�� NetworkContext �����͸� �����Ѵ�
     *  �����͸� ���� ����� NetworkContextPO _ctxt�� ����Ѵ�
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Receive(NetworkContextPO& _ctxt);


    /*!
     *  NetworkContext�� ����� �����͸� ��ȣȭ�Ѵ�
     *  ��ȣȭ �� Receive(NetworkContextPO& _ctxt)�� �����Ѵ�
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Decrypt(NetworkContextPO& _ctxt);

    /*!
     *  ���� ��� ���(m_oSendWaitingList)�� ��Ŷ�� �߰��ϴ� �Լ�
     *
     *      @param [in] _packt
     *
     *      @return
     */
    bool Waiting(Packet::SharedPtr _packt);

    /*!
     *  ��Ŷ ��ȣȭ
     *  m_oSendWaitingList�� �ִ� ���� ��� ��Ŷ �����
     *  m_oSendWorkQueue�� �̵�(swap)�ϰ�
     *  NetworkContext�� ���(write)�Ѵ�
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Encrypt(NetworkContextPO& _ctxt);

    /*!
     *  NetworkContext�� ����� �����͸� Socket ������� WSASend()�� �۽��Ѵ�
     *
     *      @param [in,out] _ctxt
     *
     *      @return
     */
    bool Send(NetworkContextPO& _ctxt);

    /*!
     *  NetworkHostPO�� ����� Socket�� �ݴ´�.
     *
     *      @param [in] _e
     *
     *      @return
     */
    bool Close(ESocketCloseType _e);
    //

    /*!
     *  ���� ��� ���� ���� Ȯ��
     *
     *      @return True if alive. False if not.
     */
    bool IsAlive();

    /*!
     *  NetworkController���� _UpdateHost()�Լ��� ���� ����ȴ�
     *  NetworkController���� Timer�� ��ϵǾ� ������(while ��)�� ����
     *  ���������� ����ȴ�.
     *
     *  NetworkHostPO�� EHostType�� ���� �Ʒ��� �Լ����� �����Ѵ�
     *  EHostType::Listener  -> UpdateListener(_appTimeMS)
     *  EHostType::Acceptor  -> UpdateAccepter(_appTimeMS)
     *  EHostType::Connector -> UpdateConnector(_appTimeMS)
     *
     *      @param [in] _appTimeMS
     */
    void Update(int64_t _appTimeMS);


    /*!
     *  NetworkHostPO�� EHostType�� Listener�� �� ����Ǵ� �Լ�.
     *  NetworkContextPO�� �Ҵ� �ް� BeginBaseTask()�� ����,
     *  m_lBaseTaskCount���� �����Ѵ�
     *
     *  �Ҵ���� NetworkContextPO�� ���ڰ����� Accept() �Լ��� �����Ѵ�.
     *  Accept() �Լ� ��� ���� false�� ��� EndBaseTask(true)�� �����Ѵ�.
     *
     *  ���� �� NetworkManager�� NetworkContext�� Release�ϱ� ���� �Լ�
     *  ReleaseContext()�� �����Ѵ�.
     *
     *      @param [in] _appTimeMS
     */
    void UpdateListener(int64_t _appTimeMS);


    /*!
     *  NetworkHostPO�� EHostType�� Acceptor�� �� ����Ǵ� �Լ�.
     *  m_bUsePacketRecvCheck ���� true�� �� ��Ŷ ���۷��� ���� �ð��� üũ�Ѵ�
     *  ���� �ð��� m_fPACKET_RECV_CHECK_COUNT_PER_SEC(30.f)�̻� �� ���
     *  ������ �����Ѵ�.
     *
     *  ��Ŷ ��� ó���ϱ� ���� BeginSendTask()�� �����Ѵ�.
     *
     *      @param [in] _appTimeMS
     */
    void UpdateAccepter(int64_t _appTimeMS);


    /*!
     *  NetworkHostPO�� EHostType�� Connector�� �� ����Ǵ� �Լ�.
     *  m_nCheckAliveMS ��(tick + DEFAULT_NETWORK_ALIVE_MS)�� ����ϰ�
     *  BeginSendTask() �Լ��� �����Ѵ�
     *
     *      @param [in] _appTimeMS
     */
    void UpdateConnector(int64_t _appTimeMS);

    /*!
     *  NetworkContextPO�� EContextType�� Join�� �� EHostType ���� Connector�� ����ǰ�
     *  NetworkContextPO�� EContextType�� Connect�� �� EHostType ���� Acceptor�� ����ȴ�
     *  setsockopt() �Լ��� optname�� IPPROTO_TCP�� ������ �ɼ��� TCP_NODELAY�� �����ϰ�
     *  NetworkHostPO�� �Ҵ� �� m_pEventSync�� OnConnect�Լ��� �����Ѵ�
     *
     *  ���ڰ����� ���� ���� EHostType�� ������� m_eHostType�� �Ҵ��Ѵ�.
     *      @param [in] _type
     */
    void EventConnect(const EHostType& _type);


    /*!
     *  NetworkHost�� NetworkControllerPO���� ����Ǿ��� �� ���� �Ǵ� �Լ�.
     *  NetworkControllerPO�� _UpdateHost(), _AddHost()���� ���ǿ� ���� ���� �ȴ�.
     */
    void EventClose();


    /*!
     *  ���� �� ��Ŷ �����͸� ��ȣȭ ���� �� ���� �Ǵ� �Լ�
     *  m_pEventSync�� OnReceive�Լ��� ����Ǹ�
     *  _AddReceive() �Լ��� tick �����Ͱ� ���ڰ����� ����Ǿ�
     *  m_nLastPacketTick�� �����Ѵ�
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
     *  ESocketCloseType�� ���� �޼����� ��ȯ�Ѵ�
     *
     *      @param [in] _e
     *
     *      @return The socket close type string.
     */
    std::wstring _GetSocketCloseTypeString(const ESocketCloseType& _e);

    /*!
     *  NetworkHost Ÿ�Կ� ���� �޼����� ��ȯ
     *
     *      @param [in] _type
     *
     *      @return The host type.
     */
    const wchar_t* _GetHostType(const EHostType& _type);

    /*!
     *  ���� ���� ��Ŷ�� MessageID ����Ʈ�� String���� ����� ����Ѵ�.
     */
    void _GetRecvHistoryStackString();

    /*!
     *  ������ network ��Ŷ �̷µ��� ��ȯ�Ѵ�
     *
     *      @param [in,out] _list
     */
    void _GetRecvHistory(std::vector<std::tuple<int, int64_t>>& _list);

    /*!
     *  ���ڰ����� ���޹��� ��Ŷ�� MessageID��
     *  Tick �����͸� ����Ѵ� (�α�)
     *
     *      @param [in] _msgID
     *      @param [in] _tick
     */
    void _AddReceive(const int& _msgID, const int64_t& _tick);
};