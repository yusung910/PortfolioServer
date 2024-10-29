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

