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
//class NetworkHostPO;
//class NetworkHostPoolPO;
class NetworkEventSync;

class NetworkManagerPO
{
    //----------------------------------------------------------
    //NetworkManagerPO Constructor, Destructor inner method
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
    //NetworkContext end
    //----------------------------------------------------------
};

