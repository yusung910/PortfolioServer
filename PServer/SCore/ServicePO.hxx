#pragma once
#include <Windows.h>
#include "Timer.h"
#include "ObjectQueue.h"

#include <Packet.h>
#include <functional>
#include <vector>

#include "InnerPacket.h"

class ServicePO : public Timer
{
private:
	HANDLE m_hThread = INVALID_HANDLE_VALUE;
	bool m_bShouldExit = false;

	// 매 틱(tick)마다 처리할 함수
	std::vector<std::function<void()>> m_oFuncList;

	// 외부 패킷 처리
	ObjectQueue<std::shared_ptr<Packet>> m_oOuterPacketQueue;
	std::unordered_map<int, std::function<bool(const Packet&)>> m_umOuterPacketHandler;

	// 내부 패킷 처리
	ObjectQueue<InnerPacket::SharedPtr> m_oInnerPacketQueue;
	std::unordered_map<int, std::function<bool(InnerPacket::SharedPtr)>> m_umInnerPacketHandler;


	// 스위칭에 사용될 변수
	std::deque<std::shared_ptr<Packet>> m_oOuterPacketWorkList;
	std::deque<std::shared_ptr<InnerPacket>> m_oInnerPacketWorkList;

public:
	ServicePO() = default;
	virtual ~ServicePO() = default;

	void Exit();
	void Push(std::shared_ptr<Packet> _packet);

	std::shared_ptr<Packet> Pop();

	bool CreateThread();
	virtual unsigned int Run();
	size_t GetPacketQueueCount();

	void AddAdditionalFunc(std::function<void()> _func);

	void RegisterHandler(const int& _msgID, std::function<bool(const Packet&)> _func);
	void UnRegisterHandler(const int& _msgID);

	void InnerPacketPush(InnerPacket::SharedPtr _data);
	void RegisterInnerHandler(const int& _protocolID, const std::function<bool(InnerPacket::SharedPtr)>& _func);

	void UnRegisterInnerHandler(const int& _protocolID);


private:
	bool _DispatchPacket(const Packet& _packet);
	void _DispatchPacketProcess();
	bool _DispatchInnerPacket(InnerPacket::SharedPtr& _packet);
	void _InnerPacketProcess();
	void _AdditionalWorkProcess();

	static unsigned int WINAPI ThreadFunc(void* _pSelf);
};

