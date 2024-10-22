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
    
    EHostType m_eHostType = EHostType::None;

    //��Ʈ��ũ ��� event ��ũ�� ���߱� ���� Ŭ������ Pacade ���·� �̿�
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


    bool Receive(NetworkContextPO& _ctxt);
    bool Decrypt(NetworkContextPO& _ctxt);

    //����
    bool Waiting(Packet::SharedPtr _packt);

    bool Encrypt(NetworkContextPO& _ctxt);
    bool Send(NetworkContextPO& _ctxt);

    bool Close(ESocketCloseType _e);
    //


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
     *  Returns the network host's recv history stack string.
     */
    void _GetRecvHistoryStackString();

    /*!
     *  ������ network ��Ŷ �̷µ��� ��ȯ�Ѵ�
     *
     *      @param [in,out] _list 
     */
    void _GetRecvHistory(std::vector<std::tuple<int, int64_t>>& _list);

    /*!
     *  ��Ʈ��ũ ��Ŷ ���ŵ� �����͸� ����Ѵ� (�α� ����)
     *
     *      @param [in] _msgID 
     *      @param [in] _tick  
     */
    void _AddReceive(const int& _msgID, const int64_t& _tick);
};

