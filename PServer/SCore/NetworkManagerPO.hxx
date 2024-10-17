#pragma once
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <WinSock2.h>
#include <Packet.h>

class NetworkContextPO;
class NetworkContextPoolPO;
//class NetworkHostPO;
//class NetworkHostPoolPO;
class NetworkEventSync;

class NetworkManagerPO
{
//Constructor, Destructor
public:
    /*!
     *  Constructor.
     *
     *      @param [in] _nReserveContext 
     */
    NetworkManagerPO(size_t _nReserveContext = 100);
    /*!
     *  Destructor.
     */
    virtual ~NetworkManagerPO();

    void ReserveContext(size_t _nReserveContext);
    size_t GetContextAllocateCount() const;
    void CreateNetwork();
    void DestroyNetwork();

//----------------------------------------------------------
//NetworkContext
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
};

