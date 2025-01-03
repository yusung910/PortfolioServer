/**
 *  @file NetworkManager.h
 *  @author YS
 *  @date 2024-10-17
 *  @project SCore
 *
 *  서버에서 네트워크 통신을 하기 위한 클래스
 */

#pragma once
#include "SCoreAPI.h"
#include "RefSingleton.h"
#include <string>
#include <vector>
#include <Packet.h>

class NetworkContextPO;
class NetworkContextPoolPO;
class NetworkHostPO;
class NetworkHostPoolPO;
class NetworkEventSync;
class NetworkStatistics;

class NetworkManagerPO;

class SCoreAPI NetworkManager : public RefSingleton<NetworkManager>
{
private:
    NetworkManagerPO* m_pManager = nullptr;

    //----------------------------------------------------------
    //NetworkManagerPO Constructor, Destructor inner method
    //----------------------------------------------------------
public:
    NetworkManager();
    ~NetworkManager();

    virtual void Init();

    void ReserveContext(size_t _reserveContext);
    size_t GetContextAllocateCount() const;
    void CreateNetwork();
    void DestroyNetwork();

    bool Connect(NetworkEventSync* _eventSync, std::string _ip, int _port, int* _pHostID = nullptr);

    bool Listen(NetworkEventSync* _eventSync, std::string _ip, int _port);

    bool Join(NetworkEventSync* _eventSync, int _ipaddr, std::string _ip, int _port, SOCKET _sock);

    bool Send(const int& _hostID, Packet::SharedPtr _packet);

    bool BroadCast(std::vector<int>& _hostIDs, Packet::SharedPtr _packet);

    bool Close(const int& _hostID);

    virtual bool CloseHost(int _hostID, const std::string& _strReason); //override

    std::string GetIP(int _hostID);

    int GetIPInt32(int _hostID);

    int GetConnectorHostID(const std::string& _ip, int _port);

    int64_t GetLastPacketTick(int _hostID);

    //----------------------------------------------------------
    //NetworkManagerPO Constructor, Destructor inner method end
    //----------------------------------------------------------

    //----------------------------------------------------------
    //NetworkContext Pool
    //----------------------------------------------------------
public:
    NetworkContextPO* AllocateContext();
    void ReleaseContext(NetworkContextPO* _ctxt);

    int GetContextAllocateCount();
    int GetContextUseCount();
    int GetContextFreeCount();

    //----------------------------------------------------------
    //NetworkContext Pool end
    //----------------------------------------------------------

    //----------------------------------------------------------
    //NetworkHost Pool begin
    //----------------------------------------------------------

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
public:
    bool RegisterWorker(NetworkHostPO* _host);
    bool DispatchWorker(NetworkHostPO* _host, NetworkContextPO* _ctxt);
    //----------------------------------------------------------
    //NetworkWorker End
    //----------------------------------------------------------


    //----------------------------------------------------------
    //NetworkStatics begin
    //----------------------------------------------------------
public:
    void OnConnect(int _hostID);
    void OnDisconnect(int _hostID);
    void OnSend(const int& _bytes);
    void OnRecv(const int& _bytes);

    bool IsConnected(const int& _hostID);

    NetworkStatistics* GetStatistics();
    void GetConnectedList(std::vector<int>& _list);

    bool IsInitialized() const;

    size_t GetContextUsingCount();

    void SetClientHostMode(const int& _hostID, const bool& _onoff);

    //----------------------------------------------------------
    //NetworkStatics End
    //----------------------------------------------------------



};

