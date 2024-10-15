/**
 *  @file NetworkCommon.h
 *  @author YS
 *  @date 2024-10-14
 *  @project SCore
 *
 *  Declares the network common.
 *  ��Ʈ��ũ ��� ���� Ŭ������ �ʿ��� ���� enum, const���� ����� Ŭ����
 */
#pragma once
#include <cstdint>

constexpr size_t NETWORK_BUFFER_SIZE_SERVER = 8192 * 2;	// ��Ŷ �ִ� ũ��
constexpr int64_t DEFAULT_NETWORK_ALIVE_MS = 15000;     // �⺻ ��Ʈ��ũ ���� �ð�(ms)
constexpr int DEFAULT_HOST_TIMEOUT_MS = 60000;          // �⺻ ȣ��Ʈ Ÿ�Ӿƿ� �ð�(ms)
constexpr int MIN_HOST_TIMEOUT_MS = 10000;              // �ּ� ȣ��Ʈ Ÿ�Ӿƿ� �ð�(ms)

enum class EContextType : int
{
    None = 0,
    Listen,
    Join,
    Accept,
    Connect,
    Receive,
    Send,
    Encrypt,
    Close,
};


enum class EHostType : int
{
    None = 0,
    Listener,		// Server Socket
    Acceptor,		// Peer Socket
    Connector,		// Client Socket
};

enum class ESocketCloseType : int
{
    Reset = 0,
    SocketTimeout,
    ManyPacketInTime,	// ª���ð��� ���̺���
    FailedToBaseTask,	// ��κ� ���� ���� ����
    FailedToSendTask,

    AddFailHostMap,
    SocketCreateFail,
    RegisterWorkerFail,
    ListenFail,
    ProcessClose,
    SendContextLimited,	// Context Limit�� �ɸ�

    ConnectFailed,	// Ŭ���̾�Ʈ���� ���� ���� ����
#ifdef MANUAL_KICK_BY_SEND_QUEUE_COUNT
    SendQueueExceed,	// ��Ŷ�� ������ ���ϰ� �׿�����. (Acceptor Host ����)

#endif
};
