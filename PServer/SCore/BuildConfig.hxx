#pragma once

// Kingdom ���� NetworkContext�� Send �۾� ������ üũ�ϴ� ����
#define CHECK_NETWORK_HOST_SEND_QUEUE_COUNT		

// ���� �̻� SendQueue ���� Ŭ���̾�Ʈ ����ű
#define MANUAL_KICK_BY_SEND_QUEUE_COUNT
#ifdef MANUAL_KICK_BY_SEND_QUEUE_COUNT
constexpr int ManualKickSendQueueCount = 1000;	// �� �̻� ���̸� ű ���
#endif