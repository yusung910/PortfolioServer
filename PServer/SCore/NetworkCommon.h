/**
 *  @file NetworkCommon.h
 *  @author YS
 *  @date 2024-10-14
 *  @project SCore
 *
 *  Declares the network common.
 *  네트워크 통신 관련 클래스에 필요한 공통 enum, const등이 선언된 클래스
 */
#pragma once
#include <cstdint>

constexpr size_t NETWORK_BUFFER_SIZE_SERVER = 8192 * 2;	// 패킷 최대 크기
constexpr int64_t DEFAULT_NETWORK_ALIVE_MS = 15000;     // 기본 네트워크 연결 시간(ms)
constexpr int DEFAULT_HOST_TIMEOUT_MS = 60000;          // 기본 호스트 타임아웃 시간(ms)
constexpr int MIN_HOST_TIMEOUT_MS = 10000;              // 최소 호스트 타임아웃 시간(ms)

enum class EContextType : int
{
    None = 0,
    Listen, //Listen : 서버가 데이터를 받을 수 있는 상태
    Join,   //
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
    ManyPacketInTime,	// 짧은시간에 많이보냄
    FailedToBaseTask,	// 대부분 유저 접속 끊음
    FailedToSendTask,

    AddFailHostMap,
    SocketCreateFail,
    RegisterWorkerFail,
    ListenFail,
    ProcessClose,
    SendContextLimited,	// Context Limit에 걸림

    ConnectFailed,	// 클라이언트모드로 서버 연결 실패
#ifdef MANUAL_KICK_BY_SEND_QUEUE_COUNT
    SendQueueExceed,	// 패킷을 보내지 못하고 쌓여있음. (Acceptor Host 전용)

#endif
};
