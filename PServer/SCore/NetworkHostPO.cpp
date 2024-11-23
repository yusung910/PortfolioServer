#include "stdafx.hxx"
#include "NetworkHostPO.hxx"

#include "NetworkSupporterPO.hxx"
#include "NetworkContextPO.hxx"
#include "PacketCompressor.hxx"

#include "NetworkManager.h"
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
        AutoLock(m_xSendLock);
        m_oSendWaitingList.clear();
    }

    m_oSendWorkQueue.clear();

    m_bIsClientHost = false;

    if (nullptr != m_pPacketCompressor.get())
        m_pPacketCompressor->Reset();

    m_eLastSocketCloseType = ESocketCloseType::Reset;

    m_bUsePacketRecvCheck = false;
    m_nPacketRecvCheckTick = 0;
    m_nPacketRecvCheckCounter = 0;

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
    if (m_oSendWorkQueue.size() == 0)
    {
        AutoLock(m_xSendLock);
        if (true == m_oSendWaitingList.empty())
            return;
    }
    //https://learn.microsoft.com/ko-kr/windows-hardware/drivers/ddi/wdm/nf-wdm-interlockedcompareexchange
    //LONG CDECL_NON_WVMPURE InterlockedCompareExchange(
    //[in, out] LONG volatile* Destination,
    //    [in]      LONG          ExChange,
    //    [in]      LONG          Comperand
    //    );
    if (InterlockedCompareExchange(&m_lSendTaskCount, 1, 0) != 0)
        return;

    //Context �Ҵ�
    auto localCtxt = NetworkManager::GetInst().AllocateContext();
    if (nullptr == localCtxt)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::BeginSendTask() Failed");
        EndSendTask(false);
        return;
    }

    localCtxt->Ready(EContextType::Encrypt);

    if (NetworkManager::GetInst().DispatchWorker(this, localCtxt) == false)
    {
        NetworkManager::GetInst().ReleaseContext(localCtxt);
        EndSendTask(false);
        return;
    }

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
    //ConnectEx �Լ��� ������ ���Ͽ� ���� ������ �����ϰ� ������ �����Ǹ� �ʿ信 ���� �����͸� �����ϴ�. ConnectEx �Լ��� ���� ���� ���Ͽ����� �����˴ϴ�.
    //���� ����: https://learn.microsoft.com/ko-kr/windows/win32/api/mswsock/nc-mswsock-lpfn_connectex
    static LPFN_CONNECTEX localConnectEx = nullptr;

    //ConnectEx�Լ� ���´�
    if (nullptr == localConnectEx)
    {
        //WSAIoctl �Լ��� ���޵� �Է� ���ۿ��� ���� 
        //ConnectEx Ȯ�� �Լ��� �ĺ��ϴ� GUID(Globally Unique Identifier)�� WSAID_CONNECTEX ���ԵǾ�� �մϴ�.
        //�����ϸ� WSAIoctl �Լ����� ��ȯ�� ��¿� ConnectEx �Լ��� ���� �����Ͱ� ���Ե�.
        // WSAID_CONNECTEX GUID�� Mswsock.h ��� ���Ͽ� ���ǵǾ� �ֽ��ϴ�.
        GUID localGUID = WSAID_CONNECTEX;

        DWORD localBytes = 0;

        // WSAIoctl() : ����, ���� �������� �Ǵ� ��� ���� �ý��ۿ� ����� � �Ű� ������ ���� �Ǵ� �˻�.
        // ���� ����� ���� ���۵� ������(LPWSAOVERLAPPED)�� �ְ� ���� �� �ִ�
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
        // [in]  s                  : ������ �ĺ��ϴ� ������.
        // [in]  dwIoControlCode    : ������ �۾��� ���� �ڵ�.
        // [in]  lpvInBuffer        : �Է� ���ۿ� ���� ������.
        // [in]  cbInBuffer         : �Է� ������ ũ��(����Ʈ).
        // [out] lpvOutBuffer       : ��� ���ۿ� ���� ������.
        // [in]  cbOutBuffer        : ��� ������ ũ��(����Ʈ).
        // [out] lpcbBytesReturned  : ���� ��� ����Ʈ ���� ���� ������.
        // [in]  lpOverlapped       : WSAOVERLAPPED ����ü�� ���� ������(��ġ�� �ʴ� ������ ��� ����).
        // [in] lpCompletionRoutine : _In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE
        // ���� ���� :https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsaioctl
        // �ι�° ���ڰ� dwIoControlCode�� ���� ����
        // https://learn.microsoft.com/ko-kr/windows/win32/winsock/winsock-ioctls
        if (WSAIoctl(m_oSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &localGUID, sizeof(localGUID), &localConnectEx, sizeof(localConnectEx), &localBytes, nullptr, nullptr) != 0)
        {
            VIEW_WRITE_ERROR(L"NetworkHost::Connect() Failed - WSAIoctl: %d", WSAGetLastError());
            return false;
        }
    }

    if (nullptr == localConnectEx)
        return false;

    //���� ��Ʈ �Ҵ�
    SOCKADDR_IN local = {};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;

    //::bind() : �����ּҸ� ���ϰ� ����.
    //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-bind
    if (::bind(m_oSocket, (SOCKADDR*)&local, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        VIEW_WRITE_ERROR(L"NetworkHost::Connect() bind:%d", WSAGetLastError());
        return false;
    }

    //�ּ� ������ �����´�
    SOCKADDR_IN localRemote = NetworkSupporterPO::GetAddressInfo(m_sIP, m_nPort);

    //Connect ��û
    _ctxt.Ready(EContextType::Connect);
    _ctxt.IncreaseReferenceCount();
    
    DWORD localDwBytesSent = 0; 

    if (localConnectEx(m_oSocket, (sockaddr*)&localRemote, sizeof(localRemote), nullptr, 0, &localDwBytesSent, &_ctxt) == FALSE)
    {
        int localConnectError = WSAGetLastError();

        // ���� ���� �ڵ� : WSA_IO_PENDING
        // ������ �����Ͱ� ���� ���� ��쿡�� ReferenceCount�� �������� �ʴ´�
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

    //Host Ÿ�� ����
    m_eHostType = EHostType::Listener;

    //Port �Ҵ�
    SOCKADDR_IN localAddr = NetworkSupporterPO::GetAddressInfo(m_sIP, m_nPort);
    if (::bind(m_oSocket, (SOCKADDR*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::Listen() Failed - bind:%d", WSAGetLastError());
        m_pEventSync->OnListenFailed();

        return false;
    }

    //Waiting ���
    //listen() : ������ ���� ��� ���·� ����.
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

    //���� ����
    //WSASocket �Լ��� Ư�� ���� ���� �����ڿ� ���ε��� ������ ����ϴ�.
    //Ư�� protocol�� ���� �� ������ �����ϴ� �Լ��� ���δ�
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
    //AcceptEx() : AcceptEx �Լ��� �� ������ �����ϰ� ���� �� ���� �ּҸ� ��ȯ�ϸ� Ŭ���̾�Ʈ ���ø����̼ǿ��� ���� ù ��° ������ ����� ����.
    // BOOL AcceptEx(
    //    [in]  SOCKET       sListenSocket,
    //    [in]  SOCKET       sAcceptSocket,
    //    [in]  PVOID        lpOutputBuffer,
    //    [in]  DWORD        dwReceiveDataLength,
    //    [in]  DWORD        dwLocalAddressLength -> ���� �ּ� ������ ���� ����� ����Ʈ ��. �� ���� ��� ���� ���� ���������� �ִ� �ּ� ���̺��� 16����Ʈ �̻��̾�� �ϱ� ������ +16�� ����,
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
        //GetAcceptExSockaddrs() : AcceptEx �Լ� ȣ�⿡�� ������ �����͸� ���� �м��ϰ� ���� �� ���� �ּҸ� sockaddr ����ü�� ����.
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

    //Receive ��û
    //WSABUF ����ü Winsock �Լ����� ����ϴ� ������ ���۸� ����ų� ��Ʈ�� �ϱ� ���� ����ü.
    WSABUF localWSABUF = {};
    DWORD localBytes = 0;
    DWORD localFlagBytes = 0;

    //�۽��ϱ� ���� NetworkContext�� ����ִ� ������ �ּ� ��ġ�� �����´�
    localWSABUF.buf = _ctxt.GetEmpty();
    //�۽��ϱ� ���� NetworkContext�� ����ִ� �޸��� ũ�⸦ �����´�
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

    //WSARecv() : ����� ���� �Ǵ� ���ε��� ���� ���� ���Ͽ��� �����͸� ����.
    //https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsarecv
    if (WSARecv(m_oSocket, &localWSABUF, 1, &localBytes, &localFlagBytes, &_ctxt, nullptr) == SOCKET_ERROR)
    {
        int localErr = WSAGetLastError();
        if (localErr != WSA_IO_PENDING)
        {
            VIEW_WRITE_ERROR(L"NetworkHostPO::Receive() Failed - WSARecv length:%d, HostID:%d, IP: %s, Error: %d"
                , static_cast<int>(localWSABUF.len)
                , GetHostID()
                , StringUtil::ToWideChar(GetIP()).c_str()
                , localErr
            );
            _ctxt.DecreaseReferenceCount();
            return false;
        }
    }

    return true;
}

bool NetworkHostPO::Decrypt(NetworkContextPO& _ctxt)
{
    //
    char* localPacket = _ctxt.GetData();
    int localPacketSize = static_cast<int>(_ctxt.GetDataSize());

    while (localPacketSize >= PACKET_HEADER_SIZE)
    {
        int localMsgSize = (*(u_long*)(localPacket)) - PACKET_HEADER_SIZE;
        int localMsgID = ntohl(*(u_long*)(localPacket + sizeof(u_long)));

        if (USE_PACKET_COMPRESS)
        {
            localMsgSize &= ~PACKET_COMPRESS_MASK;
        }

        if (localMsgSize < 0 || localMsgSize > MAX_PACKET_DATA_SIZE)
        {
            VIEW_WRITE_ERROR(
                L"NetworkHostPO::Decrypt() Failed - MessageID:%d, MessageSize:%d, HostID:%d, IPAddress:%s"
                ,localMsgID
                ,localMsgSize
                ,GetHostID()
                ,StringUtil::ToWideChar(GetIP()).c_str()
            );

            return false;
        }

        // �ܿ� ��Ŷ ���� Ȯ��
        if (localMsgSize > localPacketSize - (int)PACKET_HEADER_SIZE)
            break;

        EventReceive(localMsgID, localPacket + PACKET_HEADER_SIZE, localMsgSize);

        //��Ŷ ������ Readó��
        _ctxt.Read(static_cast<size_t>(localMsgSize) + PACKET_HEADER_SIZE);

        //������Ŷ ��ġ
        localPacket = _ctxt.GetData();
        localPacketSize = static_cast<int>(_ctxt.GetDataSize());
    }

    //Recevie ó��
    return Receive(_ctxt);
}



bool NetworkHostPO::Waiting(Packet::SharedPtr _packt)
{
    if (nullptr == _packt.get())
        return false;

#ifdef CHECK_NETWORK_HOST_SEND_QUEUE_COUNT
    int localWaitCount = 0;
    {
        AutoLock(m_xSendLock);
        m_oSendWaitingList.push_back(_packt);
        localWaitCount = static_cast<int>(m_oSendWaitingList.size());
    }

    if (localWaitCount >= 500)
    {
        if (localWaitCount % 100 == 0)
        {
            VIEW_WRITE_WARNING(
                L"NetworkHostPO::Waiting() Warning - HostID: %d [%s] Send Queue Size Over 500!! (%d)"
                , GetHostID()
                , StringUtil::ToWideChar(GetIP()).c_str()
                , localWaitCount
            );
        }
#ifdef MANUAL_KICK_BY_SEND_QUEUE_COUNT
        if (m_eHostType == EHostType::Acceptor)
        {
            if (localWaitCount > ManualKickSendQueueCount)
            {
                Close(ESocketCloseType::SendQueueExceed);
                return false;
            }
        }
#endif // MANUAL_KICK_BY_SEND_QUEUE_COUNT

    }

#else
    AutoLock(m_xSendLock);
    m_oSendWaitingList.push_back(_packt);
#endif // CHECK_NETWORK_HOST_SEND_QUEUE_COUNT


    return true;
}

bool NetworkHostPO::Encrypt(NetworkContextPO& _ctxt)
{
    size_t localTotal = 0;
    {
        AutoLock(m_xSendLock);

        //��Ŷ ���� ��� ���(m_oSendWaitingList)���� ���� ���(m_oSendWorkQueue)���� �����͸� �̵��Ѵ�
        m_oSendWaitingList.swap(m_oSendWorkQueue);
    }

    for (auto it = m_oSendWorkQueue.begin(); it != m_oSendWorkQueue.end(); )
    {
        if (nullptr != it->get())
        {
            if (localTotal + it->get()->GetPacketSize() > NETWORK_BUFFER_SIZE_SERVER)
            {
                AutoLock(m_xSendLock);
                //m_oSendWaitingList�� ��(m_oSendWaitingList.begin() ��ġ)�� 
                //m_oSendWorkQueue�� ����(begin())���� ��(end())�� �ִ´�
                m_oSendWaitingList.insert(m_oSendWaitingList.begin(), m_oSendWorkQueue.begin(), m_oSendWorkQueue.end());
                
                break;
            }

            //NetworkContext�� ����Ѵ�
            _ctxt.Write(it->get()->BinaryData, it->get()->GetPacketSize());
            localTotal += it->get()->GetPacketSize();
        }
        //���۵ǰų� nullptr�� queue�� ���� �� iterator �̵�
        it = m_oSendWorkQueue.erase(it);

    }

    m_oSendWorkQueue.clear();
    return true;
}

bool NetworkHostPO::Send(NetworkContextPO& _ctxt)
{
    if (m_oSocket == INVALID_SOCKET)
        return false;

    _ctxt.Ready(EContextType::Send);
    _ctxt.IncreaseReferenceCount();

    //Send ��û
    //WSABUF ����ü Winsock �Լ����� ����ϴ� ������ ���۸� ����ų� ��Ʈ�� �ϱ� ���� ����ü.
    WSABUF localWSABUF = {};
    DWORD localBytes = 0;
    DWORD localFlagBytes = 0;

    //NetworkContext�� ����� ������ ��Ŷ�� �����ϱ����� �о�´�
    localWSABUF.buf = _ctxt.GetData();
    localWSABUF.len = static_cast<ULONG>(_ctxt.GetDataSize());

    if (localWSABUF.len <= 0)
    {
        VIEW_WRITE_ERROR(L"NetworkHostPO::Send() Failed - WSABUF length:%d, HostID:%d, IP: %s"
            , static_cast<int>(localWSABUF.len)
            , GetHostID()
            , StringUtil::ToWideChar(GetIP()).c_str()
        );

        _ctxt.DecreaseReferenceCount();
        return false;
    }


    //WSASend() : ����� ���Ͽ� �����͸� �����ϴ�.
    // NetworkContext�� ����� buffer���� ����� socket�� send�Ѵ�
    // int WSASend (IN SOCKET s,
    //    __in_ecount(dwBufferCount) LPWSABUF lpBuffers,
    //    IN DWORD dwBufferCount,
    //    __out_opt LPDWORD lpNumberOfBytesSent,
    //    IN DWORD dwFlags,
    //    __in_opt LPWSAOVERLAPPED lpOverlapped,
    //    __in_opt LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine));
    //s: ������ �ڵ� ����, Overlapped IO �Ӽ��� �ο��� ������ �ڵ� ���޽� Overlapped IO �𵨷� ��� ����.
    //lpBuffers : ������ ������ ������ ���ϴ� WSABUF ����ü ������� �̷��� �迭�� �ּҰ� ����
    //dwBufferCount : �ι�° ���ڷ� ���޵� �迭�� �������� ����.
    //lpNumberOfBytesSent : ���۵� ����Ʈ���� ����� ������ �ּ� �� ����
    //dwFlags : �Լ��� ������ ����Ư���� �����ϴ� ��쿡 ���, ���� MSG_OOB�� �����ϸ� OOB ��� ������ ����
    //lpOverlapped : WSAOVERLAPPED ����ü ������ �ּ� �� ����, Event ������Ʈ�� ����ؼ� ������ ������ �ϷḦ Ȯ���ϴ� ��쿡 ���Ǵ� �Ű�����
    //lpCompletionRoutine : Completion Routine �̶�� �Լ��� �ּ� �� ����, �̸� ���ؼ��� ������ ������ �ϷḦ Ȯ���� �� �ִ�.
    // https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsasend
    if (WSASend(m_oSocket, &localWSABUF, 1, &localBytes, localFlagBytes, &_ctxt, nullptr) == SOCKET_ERROR)
    {
        int localWSASendError = WSAGetLastError();
        if (localWSASendError != WSA_IO_PENDING)
        {
            VIEW_WRITE_ERROR(L"NetworkHostPO::Receive() Failed - WSASend length:%d, HostID:%d, IP: %s"
                , static_cast<int>(localWSABUF.len)
                , GetHostID()
                , StringUtil::ToWideChar(GetIP()).c_str()
            );
            _ctxt.DecreaseReferenceCount();
            return false;
        }
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
        VIEW_WRITE_WARNING(L"HostID: %d, HostType: %s, Socket Close Reason : %s, [%s:%d]", m_nHostID
             , _GetHostType(m_eHostType)
             , _GetSocketCloseTypeString(_e).c_str()
             , StringUtil::ToWideChar(m_sIP).c_str()
             , m_nPort);
#else
        VIEW_WRITE_WARNING(L"HostID: %d, Socket Close Reason : %s, [%s:%d]", m_nHostID
            , _GetSocketCloseTypeString(_e).c_str()
            , StringUtil::ToWideChar(m_sIP).c_str()
            , m_nPort);
#endif // DEV_TEST

        break;
    }

    return true;
}

bool NetworkHostPO::IsAlive()
{
    if (m_oSocket != INVALID_SOCKET
        || m_lBaseTaskCount > 0
        || m_lSendTaskCount > 0)
    {
        return true;
    }

    return false;
}

void NetworkHostPO::Update(int64_t _appTimeMS)
{
    if (m_oSocket == INVALID_SOCKET)
        return;

    switch (m_eHostType)
    {
    case EHostType::Listener: UpdateListener(_appTimeMS); break;
    case EHostType::Acceptor: UpdateAccepter(_appTimeMS); break;
    case EHostType::Connector: UpdateConnector(_appTimeMS); break;
    default:
        break;
    }

}

void NetworkHostPO::UpdateListener([[maybe_unused]] int64_t _appTimeMS)
{
    int localCreateCount = ACCEPT_WAIT_COUNT - m_lBaseTaskCount;
    if (localCreateCount <= 0)
        return;

    for (int i = 0; i < localCreateCount; i++)
    {
        auto localCtxt = NetworkManager::GetInst().AllocateContext();
        if (localCtxt == nullptr)
        {
            VIEW_WRITE_ERROR(L"NetworkHostPO::UpdateListener - Allocate Context Failed");
            continue;
        }

        BeginBaseTask();

        if (Accept(*localCtxt) == false)
            EndBaseTask(true);

        NetworkManager::GetInst().ReleaseContext(localCtxt);
    }
}

void NetworkHostPO::UpdateAccepter(int64_t _appTimeMS)
{
    if (true == m_bUsePacketRecvCheck)
    {
        if (_appTimeMS > m_nPacketRecvCheckTick)
        {
            m_nPacketRecvCheckTick = _appTimeMS + m_nPACKET_RECV_CHECK_TICK;
            [[maybe_unused]] float localNetcount = m_nPacketRecvCheckCounter * 1000.0f / m_nPACKET_RECV_CHECK_TICK;
            m_nPacketRecvCheckCounter = 0;

            if (localNetcount > m_fPACKET_RECV_CHECK_COUNT_PER_SEC)
            {
                VIEW_WRITE_ERROR("Too Many Packet Close. %f (IP: %s, HostID: %d)", localNetcount, GetIP().c_str(), m_nHostID);

                _GetRecvHistoryStackString();
                Close(ESocketCloseType::ManyPacketInTime);
                return;
            }
        }
    }

    //��ȣȭ ó��
    BeginSendTask();
}

void NetworkHostPO::UpdateConnector(int64_t _appTimeMS)
{
    if (_appTimeMS > m_nCheckAliveMS)
    {
        m_nCheckAliveMS = _appTimeMS + DEFAULT_NETWORK_ALIVE_MS;
        NOTICE("Need to  Core Packet");
    }

    //��ȣȭ ó��
    BeginSendTask();
}

void NetworkHostPO::EventConnect(const EHostType& _type)
{
    int localFlag = 1;
    setsockopt(m_oSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&localFlag, sizeof(localFlag));

    if (nullptr != m_pEventSync)
    {
        //�ð� ����
        int64_t localAppTimeMS = Clock::GetTick64();
        m_nCheckTimeoutMS = localAppTimeMS + m_pEventSync->GetTimeoutMS();
        m_nCheckAliveMS = localAppTimeMS + DEFAULT_NETWORK_ALIVE_MS;

        NetworkManager::GetInst().OnConnect(m_nHostID);
        m_pEventSync->OnConnect(m_nHostID, m_sIP, 0);
    }

    m_eHostType = _type;
}

void NetworkHostPO::EventClose()
{
    if (nullptr == m_pEventSync)
        return;

    if (m_eLastSocketCloseType == ESocketCloseType::ConnectFailed)
    {
        m_pEventSync->OnConnectedFailed(m_nHostID);
    }
    else
    {
        m_pEventSync->OnClose(m_nHostID);
    }

    m_pEventSync = nullptr;
}

void NetworkHostPO::EventReceive(int _msgID, char* _msg, int _msgSize)
{
    if (nullptr == m_pEventSync)
        return;

    if (USE_PACKET_COMPRESS
        && nullptr != m_pPacketCompressor.get())
    {
        if (true == m_pPacketCompressor->Decompress(_msg, _msgSize))
        {
            m_pEventSync->OnReceive(m_nHostID, _msgID, m_pPacketCompressor->m_cCompressBuffer, m_pPacketCompressor->m_nCompressedSize);
        }
        else
        {
            m_pEventSync->OnReceive(m_nHostID, _msgID, _msg, _msgSize);
        }
    }
    else
    {
        m_pEventSync->OnReceive(m_nHostID, _msgID, _msg, _msgSize);
    }

    int64_t localAppTimeMS = Clock::GetTick64();
    m_nCheckTimeoutMS = localAppTimeMS + m_pEventSync->GetTimeoutMS();
    _AddReceive(_msgID, localAppTimeMS);

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
    // TODO: ���⿡ return ���� �����մϴ�.
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

    m_nPort = _port;
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
    //Array, Vector�� ���� https://dev-record.tistory.com/26
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