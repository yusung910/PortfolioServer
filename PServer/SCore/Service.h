#pragma once
#include "SCoreAPI.h"
#include <Packet.h>
#include <memory>
#include <functional>

#pragma warning (push)
#pragma warning (disable : 4634 26439)
#include <flatbuffers/flatbuffers.h>
#pragma warning (pop)

#include "InnerPacket.h"

class IDispatcher;
class ServicePO;

class SCoreAPI Service
{
private:
	ServicePO* m_pService = nullptr;

public:
	Service();
	virtual ~Service();
	virtual void OnDeserializeFail([[maybe_unused]] const int& _hostID, [[maybe_unused]] const std::string& _hostIP, [[maybe_unused]] const int& _failProtocol) {};

	void Exit();
	void Push(std::shared_ptr<Packet> _packet);

	bool CreateThread();
	size_t GetPacketQueueCount();
	virtual unsigned int Run();

	bool RegisterTimer(const int& _interMS, std::function<void()> _callback);

	void AddAdditionalFunc(std::function<void()> _func);
	void InnerPacketPush(InnerPacket::SharedPtr _data);

private:
	void _RegisterHandler(const int& _msgID, std::function<bool(const Packet&)> _func);
	void _RegisterInnerHandler(const int& _msgID, const std::function<bool(InnerPacket::SharedPtr)>& _func);

	bool _OnDeserializeFail(const int& _hostID, const int& _msgID);

	void _UnRegisterHandler(const int& _msgID);
	void _UnRegisterInnerHandler(const int& _protocolID);

protected:

	template <typename DispatcherType, typename MessageType,
		typename = typename std::enable_if<std::is_base_of<Service, DispatcherType>::value>::type>
	void RegisterHandler(bool (DispatcherType::* _handler)(int, const MessageType&))
	{
        if (nullptr == m_pService)
            return;

		DispatcherType* lDerived = static_cast<DispatcherType*>(this);

		int lID = static_cast<int>(typename MessageType::NativeTableType().messageid);

		auto lInvoker = [this, lID, lDerived, _handler](const Packet& _packet) {
			auto lMSG = flatbuffers::GetRoot<MessageType>(_packet.GetDataPtr());

			if (nullptr == lMSG)
				return false;

			flatbuffers::Verifier lVerifier{ (uint8_t*)_packet.GetDataPtr(), (flatbuffers::uoffset_t)_packet.GetMessageSize() };

			if (false == lMSG->Verify(lVerifier))
				return _OnDeserializeFail(_packet.m_nHostID, lID);

			return (lDerived->*_handler)(_packet.m_nHostID, *lMSG);
		};

		_RegisterHandler(lID, lInvoker);
	};


	template<typename DispatcherType, typename EnumType,
			typename = typename std::enable_if<std::is_base_of<Service, DispatcherType>::value>::type>
	void RegisterInnerHandler(const EnumType& _msgID, bool (DispatcherType::* _handler)(InnerPacket::SharedPtr))
	{
		if (nullptr == m_pService)
			return;

		int lID = static_cast<int>(_msgID);
		DispatcherType* lDerived = static_cast<DispatcherType*>(this);
		auto lInvoker = [lDerived, _handler](InnerPacket::SharedPtr _packet) {
			if (nullptr == _packet.get())
				return false;
			return (lDerived->*_handler)(_packet);
		};

		_RegisterInnerHandler(lID, lInvoker);
	}

	void UnRegisterHandler(int _msgID)
	{
		_UnRegisterHandler(_msgID);
	}

	void UnRegisterInnerHandler(int _protocolID)
	{
		_UnRegisterInnerHandler(_protocolID);
	}
};

