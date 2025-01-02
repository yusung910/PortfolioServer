/**
 *  @file NetworkManagerPO.hxx
 *  @author YS
 *  @date 2024-10-18
 *  @project SCore
 *
 *  네트워크 통신에 필요한 각각의 클래스들과
 *  외부 다른 클래스들이 접근하기 위한 클래스
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
     *  네트워크 관련 객체들을 생성한다
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
     *  NetworkManager 객체가 소멸될 때 생성자에서 생성된 객체와 관련 멤버들을 제거한다
     */
    virtual ~NetworkManagerPO();

    /*!
     *  NetworkContextPoolPO에 NetworkContextPO를 저장할 std::deque 공간을 할당한다
     *
     *      @param [in] _reserveContext
     */
    void ReserveContext(size_t _reserveContext);

    /*!
     *  NetworkContextPoolPO에 NetworkContextPO가 저장될 공간 갯수를 가져온다
     *
     *      @return The context allocate count.
     */
    size_t GetContextAllocateCount() const;

    /*!
     *  네트워크 통신을 위한 NetworkWorkerPO와 NetworkControllerPO의 스레드를 실행한다
     */
    void CreateNetwork();
    /*!
     *  네트워크 통신을 위한 NetworkWorkerPO와 NetworkControllerPO의 스레드를 종료한다
     */
    void DestroyNetwork();

    /*!
     *  NetworkHostPO을 생성하고
     *  인자값으로 전달받은 IP와 PORT을 NetworkHostPO에 할당한 NetworkHostPO를
     *  NetworkContextPO에 기록하고 NetworkController의 스레드에 Push한다
     *  이때 NetworkContextPO에 EContextType을 Connect로 지정한다
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
     *  NetworkHostPO을 생성하고
     *  인자값으로 전달받은 IP와 PORT을 NetworkHostPO에 할당한 NetworkHostPO를
     *  NetworkContextPO에 기록하고 NetworkController의 스레드에 Push한다
     *  이때 NetworkContextPO에 EContextType을 Listen로 지정한다
     *      @param [in,out] _eventSync
     *      @param [in]     _ip
     *      @param [in]     _port
     *
     *      @return
     */
    bool Listen(NetworkEventSync* _eventSync, std::string _ip, int _port);

    /*!
     *  NetworkHostPO을 생성하고
     *  인자값으로 전달받은 IP와 PORT을 NetworkHostPO에 할당한 NetworkHostPO를
     *  NetworkContextPO에 기록하고 NetworkController의 스레드에 Push한다
     *  이때 NetworkContextPO에 EContextType을 Join로 지정한다
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
     *  인자값으로 전달받은 hostID에 Packet을 전송한다.
     *  패킷 상태에 따라 압축 하고
     *  해당 패킷을 전송하기 위해 NetworkController의 SendPacketToHost()를
     *  실행한다.
     *
     *      @param [in] _hostID
     *      @param [in]
     *
     *      @return
     */
    bool Send(const int& _hostID, Packet::SharedPtr _packet);

    /*!
     *  인자값으로 전달받은 _hostIDs 배열에 저장된 모든 HostID에
     *  인자값인 Packet 데이터를 압축해서 전송한다
     *
     *      @param [in,out] _hostIDs
     *      @param [in]     _packet
     *
     *      @return
     */
    bool BroadCast(std::vector<int>& _hostIDs, Packet::SharedPtr _packet);

    /*!
     *  인자값으로 전달받은 HostID의 NetworkContext를 Close 한다
     *
     *      @param [in] _hostID
     *
     *      @return
     */
    bool Close(const int& _hostID);

    /*!
     *  NetworkHost를 Close한다
     */
    virtual bool CloseHost(int _hostID, const std::string& _strReason); //override

    /*!
     *  접속된 NetworkHostPO에 해당하는 IP 주소를 반환한다.
     *
     *      @param [in] _hostID
     *
     *      @return
     */
    std::string GetIP(int _hostID);

    /*!
     *  접속된 NetworkHostPO에 해당하는 int 타입의 IP 주소값을 반환한다
     *
     *      @param [in] _hostID
     *
     *      @return
     */
    int GetIPInt32(int _hostID);

    /*!
     *  인자 값으로 전달받은 IP와, Port로
     *  NetworkControllerPO에 연결되어 있는 HostID를 검색하여 반환한다.
     *
     *      @param [in] _ip
     *      @param [in] _port
     *
     *      @return
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
     *  NetworkControllerPO의 스레드에 NetworkContextPO를 추가한다
     *  NetworkControllerPO 스레드 추가에 실패 할 경우
     *  인자값으로 전달받은 NetworkContextPO와, NetworkHostPO를 제거 한다
     *
     *      @param [in,out] _ctxt
     *      @param [in,out] _host
     *
     *      @return
     */
    bool _DispatchController(NetworkContextPO* _ctxt, NetworkHostPO* _host = nullptr);

    /*!
     *  Packet 크기가 80Byte 이상일 경우
     *  LZ4 라이브러리를 이용해서 Packet 데이터를 압축한다
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
     *  NetworkContextPoolPO에 NetworkContextPO를 할당한다
     *
     *      @return 할당된 NetworkContextPO
     */
    NetworkContextPO* AllocateContext();

    /*!
     *  전달받은 NetworkContextPO를 초기화 하고 사용대기 상태로 전환
     *
     *      @param [in,out] _context
     */
    void ReleaseContext(NetworkContextPO* _context);

    /*!
     *  NetworkContextPoolPO에 할당된 NetworkContextPO 갯수를 반환한다
     *
     *      @return
     */
    int GetContextAllocateCount();

    /*!
     *  NetworkContextPoolPO에서 사용중인 NetworkContextPO 갯수를 반환한다
     *
     *      @return The context use count.
     */
    int GetContextUseCount();

    /*!
     *  NetworkContextPoolPO에서 사용 대기중인 NetworkContextPO 갯수를 반환한다
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
     *  m_oUsingHostIDList에 연결된 hostID를 넣고
     *  NetworkStatistics의 Connect()를 실행
     *
     *      @param [in] _hostID
     */
    void OnConnect(int _hostID);
    /*!
     *  m_oUsingHostIDList에 연결 해제된 HostID를 제거하고
     *  NetworkStatistics의 Disconnect() 실행
     *      @param [in] _hostID
     */
    void OnDisconnect(int _hostID);

    /*!
     *  송신 bytes를 statistics에 기록
     *
     *      @param [in] _bytes
     */
    void OnSend(const int& _bytes);

    /*!
     *  수신 bytes를 statistics에 기록
     *
     *      @param [in] _bytes
     */
    void OnRecv(const int& _bytes);

    /*!
     *  해당 HostID가 연결 되어있을 때 true 해제되어있을 경우 false
     *  연결 여부는 m_oUsingHostIDList vector에 있는지 없는지에 따라 확인한다
     *      @param [in] _hostID
     *
     *      @return True if connected. False if not.
     */
    bool IsConnected(const int& _hostID);

    /*!
     *  네트워크 현황 (연결, 접속 등)을 관리하는 객체를 반환하는 함수
     *
     *      @return The statistics.
     */
    NetworkStatistics* GetStatistics();

    /*!
     *  NetworkHost에 연결되어 있는 HostID 목록을 반환한다.
     *
     *      @param [in,out] _list
     */
    void GetConnectedList(std::vector<int>& _list);

    /*!
     *  Network 통신에 필요한 IOCP 세팅 초기화 여부(m_bIsNetworkInitialized)를 반환한다
     *
     *      @return True if initialized. False if not.
     */
    bool IsInitialized() const;

    /*!
     *  NetworkContextPool의 현재 사용 수 (할당 수 - 미사용 수)를 반환한다
     *
     *      @return The context using count.
     */
    size_t GetContextUsingCount();

    /*!
     *  HostID의 ClientHostMode를 설정한다
     *
     *      @param [in] _hostID
     *      @param [in] _onoff
     */
    void SetClientHostMode(const int& _hostID, const bool& _onoff);

    //----------------------------------------------------------
    //NetworkStatics end
    //----------------------------------------------------------
};

