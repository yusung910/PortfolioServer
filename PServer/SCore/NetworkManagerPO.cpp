#include "stdafx.hxx"
#include "NetworkManagerPO.hxx"

//
#include "NetworkContextPO.hxx"
#include "NetworkContextPoolPO.hxx"
//
#include "NetworkHostPO.hxx"
#include "NetworkHostPoolPO.hxx"
//
#include "NetworkWorkerPO.hxx"
//
#include "NetworkControllerPO.hxx"
//
#include "NetworkStatistics.h"

#include <iostream>
#include <atomic>
#include <memory>

#include "LZ4Compressor.h"

#include "PacketCompressor.hxx"


#define CreateContext(x)    auto x = AllocateContext();     \
                            if(x == nullptr) return false;  \


//----------------------------------------------------------
// NetworkManagerPO Constructor, Destructor inner method
//----------------------------------------------------------
NetworkManagerPO::NetworkManagerPO(size_t _reserveContext)
{
    m_pContextPool = new NetworkContextPoolPO(_reserveContext);
    m_pHostPool = new NetworkHostPoolPO();
    m_pController = new NetworkControllerPO();
    m_pWorker = new NetworkWorkerPO();
    m_pStatistics = new NetworkStatistics();
}

NetworkManagerPO::~NetworkManagerPO()
{
    SafeDelete(m_pContextPool);
    SafeDelete(m_pController);
    SafeDelete(m_pHostPool);
    SafeDelete(m_pWorker);
    SafeDelete(m_pStatistics);
}

void NetworkManagerPO::ReserveContext(size_t _reserveContext)
{
    if (nullptr == m_pContextPool)
        return;
    m_pContextPool->Reserve(_reserveContext);
}

size_t NetworkManagerPO::GetContextAllocateCount() const
{
    if (nullptr == m_pContextPool)
        return 0;
    return m_pContextPool->GetAllocatedCount();
}

void NetworkManagerPO::CreateNetwork()
{
    if (nullptr == m_pWorker)
        return;

    if (nullptr == m_pController)
        return;

    if (m_pController->CreateThread() == false)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::CreateNetwork() Failed - Create NetworkController");
    }

    if (m_pWorker->CreateThread() == false)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::CreateNetwork() Failed - Create NetworkWorker");
    }

    m_bIsNetworkInitialized.store(true);

}

void NetworkManagerPO::DestroyNetwork()
{
    m_bIsNetworkInitialized.store(false);
    if (nullptr != m_pWorker)
        m_pWorker->TerminateThread();

    if (nullptr != m_pController)
        m_pController->TerminateThread();

}

bool NetworkManagerPO::Connect(NetworkEventSync* _eventSync, std::string _ip, int _port, int* _pHostID)
{
    //NetworkHost 생성
    auto localHost = AllocateHost();
    if (localHost == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Connect() Failed - Allocate Host Error");
        return false;
    }

    localHost->SetEventSync(_eventSync);
    localHost->SetIP(_ip);
    localHost->SetPeerPort(_port);

    //무슨 용도일까? "다른 서버로의 접속은 해당 Host가 클라이언트로써 작동해야한다"
    localHost->SetClientHostMode(true);

    if (nullptr != _pHostID)
        *_pHostID = localHost->GetHostID();

    auto localCtxt = AllocateContext();
    if (localCtxt == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Connect() Failed - Allocate Context Error");
        ReleaseHost(localHost);
        return false;
    }

    //Context에 데이터 기록
    localCtxt->Ready(EContextType::Connect);
    localCtxt->Write(&localHost, sizeof(localHost));

    return _DispatchController(localCtxt, localHost);
}

bool NetworkManagerPO::Listen(NetworkEventSync* _eventSync, std::string _ip, int _port)
{
    return false;
}


bool NetworkManagerPO::Join(NetworkEventSync* _eventSync, int _ipaddr, std::string _ip, int _port, SOCKET _sock)
{
    return false;
}
bool NetworkManagerPO::_DispatchController(NetworkContextPO* _ctxt, NetworkHostPO* _host)
{
    if (nullptr == _ctxt)
        return false;

    if (false == m_pController->PushThread(_ctxt))
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::_DispatchController() Failed - NetworkController PushThread() Result is false!!");

        ReleaseContext(_ctxt);

        if (nullptr != _host)
        {
            ReleaseHost(_host);
        }

        return false;
    }
    return true;
}
void NetworkManagerPO::_CompressPacket(Packet::SharedPtr& _packet)
{
    if (!USE_PACKET_COMPRESS)
        return;

    if (nullptr == _packet)
        return;

    if (true == _packet->IsCompressed())
        return;

    if (_packet->GetPacketSize() < DEFAULT_PACKET_COMPRESS_START_SIZE)
        return;

    PacketCompressor::SharedPtr localCompressor = PacketCompressor::New();
    if (true == localCompressor->Compress(static_cast<char*>(_packet->GetDataPtr()), _packet->GetMessageSize()))
    {
        memcpy_s(_packet->GetDataPtr(), MAX_PACKET_DATA_SIZE, localCompressor->m_cCompressBuffer, localCompressor->m_nCompressedSize);
        _packet->SetPacketSize(localCompressor->m_nCompressedSize + PACKET_HEADER_SIZE);
        _packet->SetCompressed(true);
    }
}
//----------------------------------------------------------
// NetworkManagerPO Constructor, Destructor inner method end
//----------------------------------------------------------

//----------------------------------------------------------
// NetworkContextPool
//----------------------------------------------------------

NetworkContextPO* NetworkManagerPO::AllocateContext()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::AllocateContext() Failed - NetworkContext Pool is null!");
        return nullptr;
    }
    return m_pContextPool->Allocate();
}

void NetworkManagerPO::ReleaseContext(NetworkContextPO* _context)
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::ReleaseContext() Failed - NetworkContext Pool is null!");
        return;
    }

    m_pContextPool->Release(_context);
}

int NetworkManagerPO::GetContextAllocateCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetContextAllocateCount() Failed - NetworkContext Pool is null!");
        return 0;
    }
    return (int) m_pContextPool->GetAllocatedCount();
}

int NetworkManagerPO::GetContextUseCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetContextUseCount() Failed - NetworkContext Pool is null!");
        return 0;
    }
    size_t localAlloc = 0ll;
    size_t localFree = 0ll;

    m_pContextPool->GetUsage(localFree, localAlloc);

    return (int)(localAlloc - localFree);
}

int NetworkManagerPO::GetContextFreeCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetContextFreeCount() Failed - NetworkContext Pool is null!");
        return 0;
    }

    return 0;
}
//----------------------------------------------------------
//NetworkHost Pool begin
//----------------------------------------------------------

NetworkHostPO* NetworkManagerPO::AllocateHost()
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::AllocateHost() Failed - HostPool is nullptr ");
        return nullptr;
    }
    return m_pHostPool->Allocate();
}

void NetworkManagerPO::ReleaseHost(NetworkHostPO* _host)
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::ReleaseHost() Failed - HostPool is nullptr ");
        return;
    }

    m_pHostPool->Release(_host);
}

bool NetworkManagerPO::CheckHost(int _hostID)
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::CheckHost() Failed - HostPool is nullptr ");
        return false;
    }
    return m_pHostPool->Check(_hostID);
}

//----------------------------------------------------------
//NetworkHost Pool end
//----------------------------------------------------------


//----------------------------------------------------------
//NetworkWorker begin
//----------------------------------------------------------

bool NetworkManagerPO::RegisterWorker(NetworkHostPO* _host)
{
    return m_pWorker->RegisterThread(_host);
}

bool NetworkManagerPO::DispatchWorker(NetworkHostPO* _host, NetworkContextPO* _ctxt)
{
    if (m_pWorker->PushThread(_host, _ctxt) == false)
    {
        VIEW_WRITE_ERROR("Worker PushThread() is Failed");
        return false;
    }
    return true;
}

//----------------------------------------------------------
//NetworkWorker end
//----------------------------------------------------------

//----------------------------------------------------------
//NetworkStatics begin
//----------------------------------------------------------
void NetworkManagerPO::OnConnect(int _hostID)
{
    AutoLock(m_xConnectionListLock);
    auto it = m_oUsingHostIDList.find(_hostID);

    if (it != m_oUsingHostIDList.end())
        return;

    m_oUsingHostIDList.insert(_hostID);

    if (nullptr != m_pStatistics)
        m_pStatistics->Connect();

}

void NetworkManagerPO::OnDisconnect(int _hostID)
{
    AutoLock(m_xConnectionListLock);
    auto it = m_oUsingHostIDList.find(_hostID);

    if (it != m_oUsingHostIDList.end())
        return;

    m_oUsingHostIDList.erase(it);

    if (nullptr != m_pStatistics)
        m_pStatistics->Disconnect();
}

void NetworkManagerPO::OnSend(const int& _bytes)
{
    if (_bytes <= 0)
        return;
    if (nullptr != m_pStatistics)
        m_pStatistics->Send(_bytes);
}

void NetworkManagerPO::OnRecv(const int& _bytes)
{
    if (_bytes <= 0)
        return;
    if (nullptr != m_pStatistics)
        m_pStatistics->Recv(_bytes);
}

bool NetworkManagerPO::IsConnected(const int& _hostID)
{
    AutoLock(m_xConnectionListLock);
    return m_oUsingHostIDList.find(_hostID) != m_oUsingHostIDList.end(_hostID);
}

NetworkStatistics* NetworkManagerPO::GetStatistics()
{
    return m_pStatistics;
}

void NetworkManagerPO::GetConnectedList(std::vector<int>& _list)
{
    AutoLock(m_xConnectionListLock);
    _list.clear();
    _list.assign(m_oUsingHostIDList.begin(), m_oUsingHostIDList.end());
}

//----------------------------------------------------------
//NetworkStatics end
//----------------------------------------------------------