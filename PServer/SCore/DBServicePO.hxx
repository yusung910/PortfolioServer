#pragma once
#include <Windows.h>
#include <functional>
#include <vector>

#include "ObjectQueue.h"
#include "Queue"
#include "Timer.h"

#include "InnerPacket.h"

//ODBC�� �̿��Ѵ�!
class DBServicePO : public Timer
{
private:
	HANDLE m_hThread = INVALID_HANDLE_VALUE;
	bool m_ShouldExit = false;

	//������Ŷ ó��
	ObjectQueue<InnerPacket::SharedPtr> m_oPacketQueue;
	std::unordered_map<int, std::function<bool(InnerPacket::SharedPtr)>> m_umHandlers;

	int m_nServerID;

public:
	DBServicePO() = default;
	virtual ~DBServicePO() = default;

	

};

