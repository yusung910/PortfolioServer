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
};

