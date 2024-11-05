/**
 *  @file NetworkManager.h
 *  @author YS
 *  @date 2024-10-17
 *  @project SCore
 *
 *  Declares the network manager.
 */
#pragma once
#include "SCoreAPI.h"
#include "RefSingleton.h"

class NetworkContextPO;
class NetworkContextPoolPO;

class NetworkManagerPO;

class SCoreAPI NetworkManager : public RefSingleton<NetworkManager>
{
private:
    NetworkManagerPO* m_pManager;

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
    //NetworkWorker Pool begin
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
    //----------------------------------------------------------
    //NetworkStatics End
    //----------------------------------------------------------



};

