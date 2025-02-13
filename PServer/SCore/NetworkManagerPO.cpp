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


#define CreateCheckContext(x)    auto x = AllocateContext();     \
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
    auto lHost = AllocateHost();
    if (lHost == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Connect() Failed - Allocate Host Error");
        return false;
    }

    lHost->SetEventSync(_eventSync);
    lHost->SetIP(_ip);
    lHost->SetPeerPort(_port);

    //무슨 용도일까? "다른 서버로의 접속은 해당 Host가 클라이언트로써 작동해야한다"
    lHost->SetClientHostMode(true);

    if (nullptr != _pHostID)
        *_pHostID = lHost->GetHostID();

    //NetworkContextPO를 할당한다
    auto lCtxt = AllocateContext();

    if (lCtxt == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Connect() Failed - Allocate Context Error");
        //NetworkContextPO가 생성되지 않았을 경우 NetworkHost를 FreeQueue로 이동한다.
        ReleaseHost(lHost);
        return false;
    }

    //Context에 데이터 기록
    lCtxt->Ready(EContextType::Connect);
    lCtxt->Write(&lHost, sizeof(lHost));

    return _DispatchController(lCtxt, lHost);
}

bool NetworkManagerPO::Listen(NetworkEventSync* _eventSync, std::string _ip, int _port)
{
    //NetworkHost 생성
    auto lHost = AllocateHost();
    if (lHost == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Listen() Failed - Allocate Host Error");
        return false;
    }

    lHost->SetEventSync(_eventSync);
    lHost->SetIP(_ip);
    lHost->SetPeerPort(_port);

    auto lCtxt = AllocateContext();
    if (lCtxt == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Listen() Failed - Allocate Context Error");
        ReleaseHost(lHost);
        return false;
    }

    //NetworkContextPO의 ContextType을 Listen으로 변경
    lCtxt->Ready(EContextType::Listen);
    //NetworkContextPO에 NetworkHostPO 데이터를 기록한다 
    lCtxt->Write(&lHost, sizeof(lHost));

    return _DispatchController(lCtxt, lHost);
}


bool NetworkManagerPO::Join(NetworkEventSync* _eventSync, int _ipaddr, std::string _ip, int _port, SOCKET _sock)
{
    //NetworkHost 생성
    auto lHost = AllocateHost();
    if (lHost == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Join() Failed - Allocate Host Error");
        return false;
    }

    lHost->SetSocket(_sock);
    lHost->SetEventSync(_eventSync);
    lHost->SetIPInt32(_ipaddr);
    lHost->SetIP(_ip);
    lHost->SetPeerPort(_port);

    auto lCtxt = AllocateContext();
    if (lCtxt == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::Listen() Failed - Allocate Context Error");
        ReleaseHost(lHost);
        return false;
    }

    //NetworkContextPO의 ContextType을 Listen으로 변경
    lCtxt->Ready(EContextType::Join);
    //NetworkContextPO에 NetworkHostPO 데이터를 기록한다 
    lCtxt->Write(&lHost, sizeof(lHost));

    return _DispatchController(lCtxt, lHost);
}

bool NetworkManagerPO::Send(const int& _hostID, Packet::SharedPtr _packet)
{
    if (nullptr == _packet)
        return false;

    if (nullptr == m_pController)
        return false;

    _CompressPacket(_packet);

    return m_pController->SendPacketToHost(_hostID, _packet);
}

bool NetworkManagerPO::BroadCast(std::vector<int>& _hostIDs, Packet::SharedPtr _packet)
{
    if (true == _hostIDs.empty() ||
        _packet == nullptr)
    {
        VIEW_WARNING(L"NetworkManagerPO::BroadCast() Failed - Parameter is Invalid");
        return false;
    }

    if (nullptr == m_pController)
        return false;

    _CompressPacket(_packet);

    for (auto& lHostID : _hostIDs)
    {
        _packet->m_nHostID = lHostID;

        m_pController->SendPacketToHost(lHostID, _packet);
    }

    return true;
}

bool NetworkManagerPO::Close(const int& _hostID)
{
    if (_hostID == 0)
        return false;

    CreateCheckContext(lContext);

    lContext->AddHostID(_hostID);

    lContext->Ready(EContextType::Close);

    return _DispatchController(lContext);
}

bool NetworkManagerPO::CloseHost(int _hostID, const std::string& _strReason)
{
    //m_oUsingHostIDList에 HostID가 있을 경우 연결되어 있음
    if (true == IsConnected(_hostID))
    {
        if (false == _strReason.empty())
        {
            VIEW_WARNING("Close Host (%d) %s", _hostID, _strReason.c_str());
        }

        return Close(_hostID);
    }
    return false;
}

std::string NetworkManagerPO::GetIP(int _hostID)
{
    std::string retIP;
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetIP() Failed - Network Host Pool is null");
        retIP = "0.0.0.0";
        return retIP;
    }

    auto lHost = m_pHostPool->GetHost(_hostID);

    if (nullptr == lHost)
        return "";

    return lHost->GetIP();
}

int NetworkManagerPO::GetIPInt32(int _hostID)
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetIPInt32() Failed - Network Host Pool is null");
        return 0;
    }

    auto lHost = m_pHostPool->GetHost(_hostID);

    if (nullptr == lHost)
        return 0;

    return lHost->GetIPInt32();
}

int NetworkManagerPO::GetConnectorHostID(const std::string& _ip, int _port)
{
    if (nullptr == m_pController)
        return 0;
    return m_pController->GetConnectorHostID(_ip, _port);
}

int64_t NetworkManagerPO::GetLastPacketTick(int _hostID)
{
    if (m_pHostPool == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetLastPacketTick() Failed - Network Host Pool is null");
        return 0;
    }

    auto lHost = m_pHostPool->GetHost(_hostID);

    if (nullptr == lHost)
        return 0;

    return lHost->GetLastPacketTick();
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

    PacketCompressor::SharedPtr lCompressor = PacketCompressor::New();
    if (true == lCompressor->Compress(static_cast<char*>(_packet->GetDataPtr()), _packet->GetMessageSize()))
    {
        memcpy_s(_packet->GetDataPtr(), MAX_PACKET_DATA_SIZE, lCompressor->m_cCompressBuffer, lCompressor->m_nCompressedSize);
        _packet->SetPacketSize(lCompressor->m_nCompressedSize + PACKET_HEADER_SIZE);
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
    return (int)m_pContextPool->GetAllocatedCount();
}

int NetworkManagerPO::GetContextUseCount()
{
    if (nullptr == m_pContextPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::GetContextUseCount() Failed - NetworkContext Pool is null!");
        return 0;
    }
    size_t lAlloc = 0ll;
    size_t lFree = 0ll;

    m_pContextPool->GetUsage(lFree, lAlloc);

    return (int)(lAlloc - lFree);
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
    //쫌 .end()함수에 값좀 쳐넣지 마라
    return m_oUsingHostIDList.find(_hostID) != m_oUsingHostIDList.end();
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

bool NetworkManagerPO::IsInitialized() const
{
    return m_bIsNetworkInitialized.load();
}

size_t NetworkManagerPO::GetContextUsingCount()
{
    if (nullptr == m_pContextPool)
        return 0;

    size_t lAlloc = 0;
    size_t lNFree = 0;

    m_pContextPool->GetUsage(lNFree, lAlloc);

    return lAlloc - lNFree;
}

void NetworkManagerPO::SetClientHostMode(const int& _hostID, const bool& _onoff)
{
    if (nullptr == m_pHostPool)
    {
        VIEW_WRITE_ERROR("NetworkManagerPO::SetClientHostMode() Failed - Network HostPool is nullptr");
        return;
    }
    auto lHost = m_pHostPool->GetHost(_hostID);
    if (nullptr == lHost)
        return;

    lHost->SetClientHostMode(_onoff);
}

//----------------------------------------------------------
//NetworkStatics end
//----------------------------------------------------------