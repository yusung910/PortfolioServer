#include "stdafx.hxx"
#include "NetworkHostPO.hxx"

#include "NetworkSupporterPO.hxx"
#include "NetworkContextPO.hxx"
#include "PacketCompressor.hxx"

#include "NetworkEventSync.h"

#include "Clock.h"

#include <iostream>
#include <Packet.h>

NetworkHostPO::NetworkHostPO()
{
    m_pPacketCompressor = PacketCompressor::New();
}

NetworkHostPO::~NetworkHostPO()
{
    m_pPacketCompressor.reset();
}

void NetworkHostPO::Reset()
{
    Close(ESocketCloseType::Reset);
    m_nHostID = 0;
    m_eHostType = EHostType::None;

    m_pEventSync = nullptr;

    m_lBaseTaskCount = 0;
    m_lSendTaskCount = 0;

    m_nCheckTimeoutMS = 0;
    m_nCheckAliveMS = 0;

    m_sIP.clear();

    {
        AutoLock(m_oSendLock);
        m_oSendWaitingList.clear();
    }

    m_oSendWorkQueue.clear();

    m_bIsClientHost = false;

    if (nullptr != m_pPacketCompressor.get())
        m_pPacketCompressor->Reset();

    m_eLastSocketCloseType = ESocketCloseType::Reset;

    m_bUsePacketRecvCheck = false;
    m_nPacketRecvCheckTick = 0;
    m_nPacketRecvCheckCountter = 0;

    m_nMessageHistoryIdx = 0;
    m_oMessageHistory = {};

    m_nLastPacketTick = 0;
}

void NetworkHostPO::BeginBaseTask()
{
    InterlockedIncrement(&m_lBaseTaskCount);
}

void NetworkHostPO::EndBaseTask(bool _rslt, const ESocketCloseType& _type)
{
    InterlockedDecrement(&m_lBaseTaskCount);

    if (true == _rslt)
        return;

    Close(_type);
}

void NetworkHostPO::BeginSendTask()
{
    //if (m_oSendWorkQueue.size() == 0)
    //{
    //    AutoLock(m_oSendLock);
    //    if (true == m_oSendWaitingList.empty())
    //        return;
    //}
}

void NetworkHostPO::EndSendTask(bool _rslt)
{
    InterlockedExchange(&m_lSendTaskCount, 0);
    if (true == _rslt)
        return;

    Close(ESocketCloseType::FailedToSendTask);
}

bool NetworkHostPO::Connect(NetworkContextPO& _ctxt)
{
    if (m_oSocket == INVALID_SOCKET)
    {
        //
        VIEW_WRITE_ERROR(L"NetworkHost::Connect() Failed - m_oSocket is INVALID_SOCKET");
        return false;
    }

    //LPFN_CONNECTEX LpfnConnectex;
    // 
    //BOOL LpfnConnectex(
    //    [in]           SOCKET s,
    //    [in]           const sockaddr * name,
    //    [in]           int namelen,
    //    [in, optional] PVOID lpSendBuffer,
    //    [in]           DWORD dwSendDataLength,
    //    [out]          LPDWORD lpdwBytesSent,
    //    [in]           LPOVERLAPPED lpOverlapped
    //)
    //ConnectEx 함수는 지정된 소켓에 대한 연결을 설정하고 연결이 설정되면 필요에 따라 데이터를 보냅니다. ConnectEx 함수는 연결 지향 소켓에서만 지원됩니다.
    //세부 설명: https://learn.microsoft.com/ko-kr/windows/win32/api/mswsock/nc-mswsock-lpfn_connectex
    static LPFN_CONNECTEX localConnectEx = nullptr;

    //ConnectEx함수 얻어온다
    if (nullptr == localConnectEx)
    {
        //WSAIoctl 함수에 전달된 입력 버퍼에는 값이 
        //ConnectEx 확장 함수를 식별하는 GUID(Globally Unique Identifier)인 WSAID_CONNECTEX 포함되어야 합니다.
        //성공하면 WSAIoctl 함수에서 반환된 출력에 ConnectEx 함수에 대한 포인터가 포함됨.
        // WSAID_CONNECTEX GUID는 Mswsock.h 헤더 파일에 정의되어 있습니다.
        GUID localGUID = WSAID_CONNECTEX;

        DWORD localBytes = 0;

        //WSAIoctl() : 소켓의 모드를 제어한다.
        //int WSAAPI WSAIoctl(
        //    [in]  SOCKET                             s,
        //    [in]  DWORD                              dwIoControlCode,
        //    [in]  LPVOID                             lpvInBuffer,
        //    [in]  DWORD                              cbInBuffer,
        //    [out] LPVOID                             lpvOutBuffer,
        //    [in]  DWORD                              cbOutBuffer,
        //    [out] LPDWORD                            lpcbBytesReturned,
        //    [in]  LPWSAOVERLAPPED                    lpOverlapped,
        //    [in]  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
        //);
        // [in]  s                  : 소켓을 식별하는 설명자.
        // [in]  dwIoControlCode    : 수행할 작업의 제어 코드.
        // [in]  lpvInBuffer        : 입력 버퍼에 대한 포인터.
        // [in]  cbInBuffer         : 입력 버퍼의 크기(바이트).
        // [out] lpvOutBuffer       : 출력 버퍼에 대한 포인터.
        // [in]  cbOutBuffer        : 출력 버퍼의 크기(바이트).
        // [out] lpcbBytesReturned  : 실제 출력 바이트 수에 대한 포인터.
        // [in]  lpOverlapped       : WSAOVERLAPPED 구조체에 대한 포인터(겹치지 않는 소켓의 경우 무시).
        // [in] lpCompletionRoutine : _In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE
        // 세부 설명 :https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsaioctl
        if (WSAIoctl(m_oSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &localGUID, sizeof(localGUID), &localConnectEx, sizeof(localConnectEx), &localBytes, nullptr, nullptr) != 0)
        {
            VIEW_WRITE_ERROR(L"NetworkHost::Connect() Failed - WSAIoctl: %d", WSAGetLastError());
            return false;
        }
    }

    if (nullptr == localConnectEx)
        return false;

    //로컬 포트 할당
    SOCKADDR_IN local = {};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;

    //::bind() : 로컬주소를 소켓과 연결.
    //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-bind
    if (::bind(m_oSocket, (SOCKADDR*)&local, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        VIEW_WRITE_ERROR(L"NetworkHost::Connect() bind:%d", WSAGetLastError());
        return false;
    }

    //주소 정보를 가져온다
    SOCKADDR_IN localRemote = NetworkSupporterPO::GetAddressInfo(m_sIP, m_nPort);

    //Connect 요청
    _ctxt.Ready(EContextType::Connect);
    _ctxt.IncreaseReferenceCount();
    
    DWORD localDwBytesSent = 0; 

    if (localConnectEx(m_oSocket, (sockaddr*)&localRemote, sizeof(localRemote), nullptr, 0, &localDwBytesSent, &_ctxt) == FALSE)
    {
        int localConnectError = WSAGetLastError();

        // 소켓 오류 코드 : WSA_IO_PENDING
        // 전송할 데이터가 남아 있을 경우에는 ReferenceCount를 감소하지 않는다
        // https://learn.microsoft.com/ko-kr/windows/win32/winsock/windows-sockets-error-codes-2
        if (localConnectError != WSA_IO_PENDING)
        {
            _ctxt.DecreaseReferenceCount();
            return false;
        }
    }
    return true;
}

bool NetworkHostPO::Listen()
{
    if (nullptr == m_pEventSync)
        return false;

    //Host 타입 세팅
    m_eHostType = EHostType::Listener;

    //Port 할당
    SOCKADDR_IN localAddr = NetworkSupporterPO::GetAddressInfo(m_sIP, m_nPort);
    if (::bind(m_oSocket, (SOCKADDR*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::Listen() Failed - bind:%d", WSAGetLastError());
        m_pEventSync->OnListenFailed();

        return false;
    }

    //Waiting 대기
    //listen() : 소켓을 수신 대기 상태로 지정.
    //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-listen
    if (listen(m_oSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::Listen() Failed - listen:%d", WSAGetLastError());
        m_pEventSync->OnListenFailed();
        return false;
    }

    m_pEventSync->OnListen();

    return true;
}


bool NetworkHostPO::Accept(NetworkContextPO& _ctxt)
{
    if (m_oSocket == INVALID_SOCKET)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::Accept() Failed - m_oSocket:%d", WSAGetLastError());
        return false;
    }

    //소켓 생성
    //WSASocket 함수는 특정 전송 서비스 공급자에 바인딩된 소켓을 만듭니다.
    //특정 protocol에 서비스 할 소켓을 생성하는 함수로 보인다
    //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsasocketw
    SOCKET localSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (localSock == INVALID_SOCKET)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::Accept() Failed - WSASocket:%d", WSAGetLastError());

        return false;
    }

    _ctxt.IncreaseReferenceCount();
    _ctxt.Ready(EContextType::Accept);
    _ctxt.Write(&localSock, sizeof(localSock));

    //Accept
    DWORD localBytes;
    //AcceptEx() : AcceptEx 함수는 새 연결을 수락하고 로컬 및 원격 주소를 반환하며 클라이언트 애플리케이션에서 보낸 첫 번째 데이터 블록을 수신.
    // BOOL AcceptEx(
    //    [in]  SOCKET       sListenSocket,
    //    [in]  SOCKET       sAcceptSocket,
    //    [in]  PVOID        lpOutputBuffer,
    //    [in]  DWORD        dwReceiveDataLength,
    //    [in]  DWORD        dwLocalAddressLength,
    //    [in]  DWORD        dwRemoteAddressLength,
    //    [out] LPDWORD      lpdwBytesReceived,
    //    [in]  LPOVERLAPPED lpOverlapped
    //    );
    //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-acceptex
    if (AcceptEx(m_oSocket, localSock, _ctxt.GetEmpty(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &localBytes, &_ctxt) == FALSE)
    {
        int localError = WSAGetLastError();
        if (localError != WSA_IO_PENDING)
        {
            VIEW_WRITE_ERROR(L"NetworkHostPO::Accept() Failed - AcceptEx:%d", localError);

            closesocket(localSock);
            _ctxt.DecreaseReferenceCount();
            return false;
        }

        int localSockAddrLen = 0;
        int localSockRemoteAddrLen = 0;
        //GetAcceptExSockaddrs() : AcceptEx 함수 호출에서 가져온 데이터를 구문 분석하고 로컬 및 원격 주소를 sockaddr 구조체에 전달.
        GetAcceptExSockaddrs(_ctxt.GetEmpty(), 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, (sockaddr**)&_ctxt.m_pLocalAddr, &localSockAddrLen, (sockaddr**)&_ctxt.m_pRemoteAddr, & localSockRemoteAddrLen);
    }

    return true;
}

bool NetworkHostPO::Receive(NetworkContextPO& _ctxt)
{
    if (m_oSocket == INVALID_SOCKET)
        return false;

    _ctxt.Ready(EContextType::Receive);
    _ctxt.IncreaseReferenceCount();

    //Receive 요청
    //WSABUF 구조체 Winsock 함수에서 사용하는 데이터 버퍼를 만들거나 컨트롤 하기 위한 구조체.
    WSABUF localWSABUF = {};
    DWORD localBytes = 0;
    DWORD flagBytes = 0;

    localWSABUF.buf = _ctxt.GetData();
    localWSABUF.len = static_cast<ULONG>(_ctxt.GetEmptySize());

    if (localWSABUF.len <= 0)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::Receive() Failed - WSABUF length:%d, HostID:%d, IP: %s"
            , static_cast<int>(localWSABUF.len)
            , GetHostID()
            , StringUtil::ToWideChar(GetIP()).c_str()
        );

        _ctxt.DecreaseReferenceCount();
        return false;
    }

    //WSASend() : 연결된 소켓에 데이터를 보냅니다.
    // NetworkContext에 저장된 buffer들을 연결된 socket에 send한다
    // https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsasend
    if (WSASend(m_oSocket, &localWSABUF, 1, &localBytes, flagBytes, &_ctxt, nullptr) == SOCKET_ERROR)
    {

    }

    return true;
}



bool NetworkHostPO::Close(ESocketCloseType _e)
{
    if (m_oSocket == INVALID_SOCKET)
        return false;

    m_eLastSocketCloseType = _e;

    LINGER localOpt = {};
    localOpt.l_onoff = 1;
    localOpt.l_linger = 0;

    setsockopt(m_oSocket, SOL_SOCKET, SO_LINGER, (char*)&localOpt, sizeof(localOpt));

    closesocket(m_oSocket);
    m_oSocket = INVALID_SOCKET;

    switch (_e)
    {
    case ESocketCloseType::FailedToSendTask:
    case ESocketCloseType::ProcessClose:
    case ESocketCloseType::ConnectFailed:
#ifndef DEV_TEST
        break;
#endif // !DEV_TEST
    default:
#ifdef DEV_TEST
        //VIEW_WRITE_WARNING(L"HostID: %d, HostType: %s, Socket Close Reason : %s, [%s:%d]", m_nHostID, );
#endif // DEV_TEST

        break;
    }

    return true;
}

const int& NetworkHostPO::GetHostID() const
{
    return m_nHostID;
}

void NetworkHostPO::SetHostID(const int& _id)
{
    if (_id < 0)
        return;

    m_nHostID = _id;
}

SOCKET NetworkHostPO::GetSocket()
{
    return m_oSocket;
}

void NetworkHostPO::SetSocket(SOCKET _sock)
{
    m_oSocket = _sock;
}

const EHostType& NetworkHostPO::GetHostType() const
{
    return m_eHostType;
}

void NetworkHostPO::SetHostType(const EHostType& _type)
{
    m_eHostType = _type;
}

NetworkEventSync* NetworkHostPO::GetEvnetSync()
{
    return m_pEventSync;
}

void NetworkHostPO::SetEventSync(NetworkEventSync* _eventSync)
{
    m_pEventSync = _eventSync;
}

std::string& NetworkHostPO::GetIP()
{
    // TODO: 여기에 return 문을 삽입합니다.
    return m_sIP;
}

void NetworkHostPO::SetIP(std::string _ip)
{
    if (true == _ip.empty())
        return;
    m_sIP.assign(_ip.begin(), _ip.end());
}

int NetworkHostPO::GetIPInt32()
{
    return m_nIP;
}

void NetworkHostPO::SetIPInt32(int _n)
{
    m_nIP = _n;
}

int64_t NetworkHostPO::GetLastPacketTick()
{
    return m_nLastPacketTick;
}

int NetworkHostPO::GetPeerPort()
{
    return m_nPort;
}

void NetworkHostPO::SetPeerPort(const int& _port)
{
    if (_port <= 0 || _port >= 65536)
        return;

    m_nPort;
}

void NetworkHostPO::SetClientHostMode(const bool& _onoff)
{
    m_bIsClientHost = _onoff;
}

std::wstring NetworkHostPO::_GetSocketCloseTypeString(const ESocketCloseType& _e)
{
    switch (_e)
    {
    case ESocketCloseType::Reset:
        return L"Host Reset";
    case ESocketCloseType::SocketTimeout:
        return L"Socket Timeout";
    case ESocketCloseType::ManyPacketInTime:
        return L"Too Many Packet Send";
    case ESocketCloseType::FailedToBaseTask:
        return L"Failed To Base Task (Close by Client)";
    case ESocketCloseType::FailedToSendTask:
        return L"Failed To Send Task";
    case ESocketCloseType::AddFailHostMap:
        return L"Add Failed To Host Map (System Error)";
    case ESocketCloseType::SocketCreateFail:
        return L"Socket Create Faile (System Error)";
    case ESocketCloseType::RegisterWorkerFail:
        return L"Register Worker Fail! (System Error)";
    case ESocketCloseType::ListenFail:
        return L"Listen Fail! (System Error)";
    case ESocketCloseType::ProcessClose:
        return L"Close by PreocessClose. (Close By Server)";
    case ESocketCloseType::SendContextLimited:
        return L"Send Context Limited (Close by Server)";
    case ESocketCloseType::SendQueueExceed:
        return L"Send Queue Count Exceed! (Close by Server)";
    default:
        break;
    }
    return L"Unknown Type : " + std::to_wstring((int)_e);
}

const wchar_t* NetworkHostPO::_GetHostType(const EHostType& _type)
{
    switch (_type)
    {
    case EHostType::Listener: return L"Server Listener";    // Server Socket
    case EHostType::Acceptor: return L"Peer Socket";        // Peer Socket
    case EHostType::Connector: return L"Client Connector";  // Client Socket
    default:
        break;
    }
    return L"None";
}

void NetworkHostPO::_GetRecvHistoryStackString()
{
    //Array, Vector의 차이 https://dev-record.tistory.com/26
    std::vector<std::tuple<int, int64_t>> localVecList;

    _GetRecvHistory(localVecList);

    std::string localStrRet = "Message Stack [";
    int localMsgID = 0;
    int64_t localInterval = 0;

    int localCount = 0;
    for (const auto& it : localVecList)
    {
        localMsgID = std::get<0>(it);
        localInterval = std::get<1>(it);

        if (0 == localMsgID) continue;

        localStrRet.append(std::to_string(localMsgID));
        localStrRet.append(" ");
        localStrRet.append(std::to_string(localInterval));
        localStrRet.append(" ms");

        if (++localCount >= 10)
        {
            localStrRet.append("\n");
            localCount = 0;
        }
        else
        {
            localStrRet.append(",");
        }
    }
    localStrRet.append("]");

    VIEW_WRITE_ERROR("HostID : %d ReceiveHistory : %s", m_nHostID, localStrRet.c_str());
}

void NetworkHostPO::_GetRecvHistory(std::vector<std::tuple<int, int64_t>>& _list)
{
    _list.clear();
    for (int i = m_nMessageHistoryIdx - 1; i >= m_nMessageHistoryIdx.load(); --i)
        _list.push_back(m_oMessageHistory[i]);

    for (int i = MAX_MESSAGE_ID_HISTORY_SIZE - 1; i >= m_nMessageHistoryIdx.load(); --i)
        _list.push_back(m_oMessageHistory[i]);

}

void NetworkHostPO::_AddReceive(const int& _msgID, const int64_t& _tick)
{
    std::get<0>(m_oMessageHistory[m_nMessageHistoryIdx.load()]) = _msgID;
    std::get<1>(m_oMessageHistory[m_nMessageHistoryIdx.load()]) = m_nLastPacketTick != 0 ? _tick - m_nLastPacketTick : 0;

    m_nMessageHistoryIdx.store((m_nMessageHistoryIdx.load() + 1) % MAX_MESSAGE_ID_HISTORY_SIZE);

    m_nLastPacketTick = _tick;
}