#pragma once

// Kingdom 에서 NetworkContext의 Send 작업 수량을 체크하는 역할
#define CHECK_NETWORK_HOST_SEND_QUEUE_COUNT		

// 일정 이상 SendQueue 쌓인 클라이언트 수동킥
#define MANUAL_KICK_BY_SEND_QUEUE_COUNT
#ifdef MANUAL_KICK_BY_SEND_QUEUE_COUNT
constexpr int ManualKickSendQueueCount = 1000;	// 이 이상 쌓이면 킥 대상
#endif