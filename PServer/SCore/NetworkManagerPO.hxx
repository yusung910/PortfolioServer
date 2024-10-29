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
public:
    /*!
     *  Constructor.
     *
     *      @param [in] _reserveContext 
     */
    NetworkManagerPO(size_t _reserveContext = 100);
    /*!
     *  Destructor.
     */
    virtual ~NetworkManagerPO();

    void ReserveContext(size_t _reserveContext);
    size_t GetContextAllocateCount() const;
    void CreateNetwork();
    void DestroyNetwork();
    //----------------------------------------------------------
    //NetworkManagerPO Constructor, Destructor inner method end
    //----------------------------------------------------------

    //----------------------------------------------------------
    //NetworkContext Pool begin
    //----------------------------------------------------------
private:
    NetworkContextPoolPO* m_pContextPool = nullptr;

public:
    NetworkContextPO* AllocateContext();
    void ReleaseContext(NetworkContextPO* _context);

    int GetContextAllocateCount();
    int GetContextUseCount();
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
    NetworkHostPO* AllocateHost();
    void ReleaseHost(NetworkHostPO* _host);
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
    bool RegisterWorker(NetworkHostPO* _host);
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

    NetworkStatistics* GetStatistics();
    void GetConnectedList(std::vector<int>& _list);

    //----------------------------------------------------------
    //NetworkStatics end
    //----------------------------------------------------------
};

