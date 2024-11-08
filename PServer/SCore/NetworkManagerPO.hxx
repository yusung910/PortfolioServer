/**
 *  @file NetworkManagerPO.hxx
 *  @author YS
 *  @date 2024-10-18
 *  @project SCore
 *
 *  ��Ʈ��ũ ��ſ� �ʿ��� ������ Ŭ�������
 *  �ܺ� �ٸ� Ŭ�������� �����ϱ� ���� Ŭ����
 *  
 *  other Class <-> NetworkManagerPO <-> NetworkContextPO, NetworkContextPoolPO ...
 */
#pragma once
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <WinSock2.h>
#include <Packet.h>

class NetworkContextPO;
class NetworkContextPoolPO;
class NetworkHostPO;
class NetworkHostPoolPO;
class NetworkEventSync;
class NetworkControllerPO;
class NetworkWorkerPO;

class NetworkStatistics;

class NetworkManagerPO
{
    //----------------------------------------------------------
    //NetworkManagerPO Constructor, Destructor inner method begin
    //----------------------------------------------------------
private:
    NetworkControllerPO* m_pController;
    std::atomic_bool m_bIsNetworkInitialized = false;

public:
    /*!
     *  ��Ʈ��ũ ���� ��ü���� �����Ѵ�
     *  
     *  new NetworkContextPoolPO(_reserveContext);
     *  new NetworkHostPoolPO();
     *  new NetworkControllerPO();
     *  new NetworkWorkerPO();
     *  new NetworkStatistics();
     *
     *      @param [in] _reserveContext 
     */
    NetworkManagerPO(size_t _reserveContext = 100);

    /*!
     *  Destructor
     *  NetworkManager ��ü�� �Ҹ�� �� �����ڿ��� ������ ��ü�� ���� ������� �����Ѵ�
     */
    virtual ~NetworkManagerPO();

    /*!
     *  NetworkContextPoolPO�� NetworkContextPO�� ������ std::deque ������ �Ҵ��Ѵ�
     *
     *      @param [in] _reserveContext 
     */
    void ReserveContext(size_t _reserveContext);

    /*!
     *  NetworkContextPoolPO�� NetworkContextPO�� ����� ���� ������ �����´�
     *
     *      @return The context allocate count.
     */
    size_t GetContextAllocateCount() const;

    /*!
     *  ��Ʈ��ũ ����� ���� NetworkWorkerPO�� NetworkControllerPO�� �����带 �����Ѵ�
     */
    void CreateNetwork();
    /*!
     *  ��Ʈ��ũ ����� ���� NetworkWorkerPO�� NetworkControllerPO�� �����带 �����Ѵ�
     */
    void DestroyNetwork();

    /*!
     *  NetworkHostPO�� �����ϰ�
     *  ���ڰ����� ���޹��� IP�� PORT�� NetworkHostPO�� �Ҵ��� NetworkHostPO��
     *  NetworkContextPO�� ����ϰ� NetworkController�� �����忡 Push�Ѵ�
     *  �̶� NetworkContextPO�� EContextType�� Connect�� �����Ѵ�
     *
     *      @param [in,out] _eventSync
     *      @param [in]     _ip
     *      @param [in]     _port
     *      @param [in,out] _pHostID
     *
     *      @return
     */
    bool Connect(NetworkEventSync* _eventSync, std::string _ip, int _port, int* _pHostID = nullptr);

    /*!
     *  NetworkHostPO�� �����ϰ�
     *  ���ڰ����� ���޹��� IP�� PORT�� NetworkHostPO�� �Ҵ��� NetworkHostPO��
     *  NetworkContextPO�� ����ϰ� NetworkController�� �����忡 Push�Ѵ�
     *  �̶� NetworkContextPO�� EContextType�� Listen�� �����Ѵ�
     *      @param [in,out] _eventSync 
     *      @param [in]     _ip        
     *      @param [in]     _port      
     *
     *      @return 
     */
    bool Listen(NetworkEventSync* _eventSync, std::string _ip, int _port);

    /*!
     *  Joins the network manager.
     *
     *      @param [in,out] _eventSync
     *      @param [in]     _ipaddr
     *      @param [in]     _ip
     *      @param [in]     _port
     *      @param [in]     _sock
     *
     *      @return
     */
    bool Join(NetworkEventSync* _eventSync, int _ipaddr, std::string _ip, int _port, SOCKET _sock);

    /*!
     *  Sends the network manager.
     *
     *      @param [in] _hostID 
     *      @param [in]         
     *
     *      @return 
     */
    bool Send(const int& _hostID, Packet::SharedPtr);

    /*!
     *  Broads the cast.
     *
     *      @param [in,out] _hostIDs 
     *      @param [in]     _packet  
     *
     *      @return 
     */
    bool BroadCast(std::vector<int>& _hostIDs, Packet::SharedPtr _packet);

    /*!
     *  Closes the network manager.
     *
     *      @param [in] _hostID 
     *
     *      @return 
     */
    bool Close(const int& _hostID);

    /*!
     *  
     */
    virtual bool CloseHost(int _hostID, const std::string& _strReason); //override

    /*!
     *  Returns the network manager's i p.
     *
     *      @param [in] _hostID 
     *
     *      @return The i p.
     */
    std::string GetIP(int _hostID);

    /*!
     *  Returns the network manager's i p int32.
     *
     *      @param [in] _hostID 
     *
     *      @return The i p int32.
     */
    int GetIPInt32(int _hostID);

    /*!
     *  Returns the network manager's connector host i d.
     *
     *      @param [in] _ip   
     *      @param [in] _port 
     *
     *      @return The connector host i d.
     */
    int GetConnectorHostID(const std::string& _ip, int _port);

    /*!
     *  Returns the network manager's last packet tick.
     *
     *      @param [in] _hostID 
     *
     *      @return The last packet tick.
     */
    int64_t GetLastPacketTick(int _hostID);


private:
    /*!
     *  NetworkControllerPO�� �����忡 NetworkContextPO�� �߰��Ѵ�
     *  NetworkControllerPO ������ �߰��� ���� �� ���
     *  ���ڰ����� ���޹��� NetworkContextPO��, NetworkHostPO�� ���� �Ѵ�
     *
     *      @param [in,out] _ctxt 
     *      @param [in,out] _host 
     *
     *      @return 
     */
    bool _DispatchController(NetworkContextPO* _ctxt, NetworkHostPO* _host = nullptr);

    /*!
     *  Packet ũ�Ⱑ 80Byte �̻��� ���
     *  LZ4 ���̺귯���� �̿��ؼ� Packet �����͸� �����Ѵ�
     *  
     *      @param [in,out] _packet 
     */
    void _CompressPacket(Packet::SharedPtr& _packet);

    //----------------------------------------------------------
    //NetworkManagerPO Constructor, Destructor inner method end
    //----------------------------------------------------------

    //----------------------------------------------------------
    //NetworkContext Pool begin
    //----------------------------------------------------------
private:
    NetworkContextPoolPO* m_pContextPool = nullptr;

public:
    /*!
     *  NetworkContextPoolPO�� NetworkContextPO�� �Ҵ��Ѵ�
     *
     *      @return �Ҵ�� NetworkContextPO
     */
    NetworkContextPO* AllocateContext();

    /*!
     *  ���޹��� NetworkContextPO�� �ʱ�ȭ �ϰ� ����� ���·� ��ȯ
     *
     *      @param [in,out] _context 
     */
    void ReleaseContext(NetworkContextPO* _context);

    /*!
     *  NetworkContextPoolPO�� �Ҵ�� NetworkContextPO ������ ��ȯ�Ѵ�
     *
     *      @return
     */
    int GetContextAllocateCount();

    /*!
     *  NetworkContextPoolPO���� ������� NetworkContextPO ������ ��ȯ�Ѵ�
     *
     *      @return The context use count.
     */
    int GetContextUseCount();

    /*!
     *  NetworkContextPoolPO���� ��� ������� NetworkContextPO ������ ��ȯ�Ѵ�
     *
     *      @return The context free count.
     */
    int GetContextFreeCount();

    //----------------------------------------------------------
    //NetworkContext Pool end
    //----------------------------------------------------------

    //----------------------------------------------------------
    //NetworkHost Pool begin
    //----------------------------------------------------------
private:
    NetworkHostPoolPO* m_pHostPool = nullptr;

public:
    /*!
     *  Allocates the host.
     *
     *      @return 
     */
    NetworkHostPO* AllocateHost();

    /*!
     *  Releases the host.
     *
     *      @param [in,out] _host 
     */
    void ReleaseHost(NetworkHostPO* _host);

    /*!
     *  Checks the host.
     *
     *      @param [in] _hostID 
     *
     *      @return 
     */
    bool CheckHost(int _hostID);

    //----------------------------------------------------------
    //NetworkHost Pool end
    //----------------------------------------------------------


    //----------------------------------------------------------
    //NetworkWorker begin
    //----------------------------------------------------------
private:
    NetworkWorkerPO* m_pWorker = nullptr;
public:
    /*!
     *  Registers the worker.
     *
     *      @param [in,out] _host 
     *
     *      @return 
     */
    bool RegisterWorker(NetworkHostPO* _host);

    /*!
     *  Dispatches the worker.
     *
     *      @param [in,out] _host 
     *      @param [in,out] _ctxt 
     *
     *      @return 
     */
    bool DispatchWorker(NetworkHostPO* _host, NetworkContextPO* _ctxt);

    //----------------------------------------------------------
    //NetworkWorker end
    //----------------------------------------------------------


    //----------------------------------------------------------
    //NetworkStatics begin
    //----------------------------------------------------------
private:
    NetworkStatistics* m_pStatistics = nullptr;
    std::unordered_set<int> m_oUsingHostIDList;
    std::mutex m_xConnectionListLock;
public:
    /*!
     *  m_oUsingHostIDList�� ����� hostID�� �ְ�
     *  NetworkStatistics�� Connect()�� ����
     *
     *      @param [in] _hostID 
     */
    void OnConnect(int _hostID);
    /*!
     *  m_oUsingHostIDList�� ���� ������ HostID�� �����ϰ�
     *  NetworkStatistics�� Disconnect() ����
     *      @param [in] _hostID 
     */
    void OnDisconnect(int _hostID);

    /*!
     *  �۽� bytes�� statistics�� ���
     *
     *      @param [in] _bytes 
     */
    void OnSend(const int& _bytes);

    /*!
     *  ���� bytes�� statistics�� ���
     *
     *      @param [in] _bytes 
     */
    void OnRecv(const int& _bytes);

    /*!
     *  �ش� HostID�� ���� �Ǿ����� �� true �����Ǿ����� ��� false
     *  ���� ���δ� m_oUsingHostIDList vector�� �ִ��� �������� ���� Ȯ���Ѵ�
     *      @param [in] _hostID 
     *
     *      @return True if connected. False if not.
     */
    bool IsConnected(const int& _hostID);

    NetworkStatistics* GetStatistics();
    void GetConnectedList(std::vector<int>& _list);

    //----------------------------------------------------------
    //NetworkStatics end
    //----------------------------------------------------------
};

