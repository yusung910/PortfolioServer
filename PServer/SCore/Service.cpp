#include "stdafx.hxx"
#include "Service.h"
#include "ServicePO.hxx"
#include "DispatcherInterface.h"
#include "NetworkManager.h"
Service::Service()
{
	m_pService = new ServicePO();
}

Service::~Service()
{
	Exit();
	SafeDelete(m_pService);
}

void Service::Exit()
{
	if (nullptr != m_pService)
		m_pService->Exit();
}

void Service::Push(std::shared_ptr<Packet> _packet)
{
	if (nullptr != m_pService)
		m_pService->Push(_packet);
}

bool Service::CreateThread()
{
	if (nullptr == m_pService)
		return false;

	return m_pService->CreateThread();
}

size_t Service::GetPacketQueueCount()
{
	if (nullptr == m_pService)
		return 0;
	return m_pService->GetPacketQueueCount();
}

unsigned int Service::Run()
{
	if (nullptr == m_pService)
		return 1;
	return m_pService->Run();
}

bool Service::RegisterTimer(const int& _interMS, std::function<void()> _callback)
{
	if (nullptr == m_pService)
		return false;

	return m_pService->RegisterTimer(_interMS, _callback);
}

void Service::AddAdditionalFunc(std::function<void()> _func)
{
	if (nullptr != m_pService)
		m_pService->AddAdditionalFunc(_func);
}

void Service::InnerPacketPush(InnerPacket::SharedPtr _data)
{
	if (nullptr != m_pService)
		m_pService->InnerPacketPush(_data);
}

void Service::_RegisterHandler(const int& _msgID, std::function<bool(const Packet&)> _func)
{
	if (nullptr != m_pService)
		m_pService->RegisterHandler(_msgID, _func);
}

void Service::_RegisterInnerHandler(const int& _msgID, const std::function<bool(InnerPacket::SharedPtr)>& _func)
{
	if (nullptr != m_pService)
		m_pService->RegisterInnerHandler(_msgID, _func);
}

bool Service::_OnDeserializeFail(const int& _hostID, const int& _msgID)
{
	std::string localIP = NetworkManager::GetInst().GetIP(_hostID);
	OnDeserializeFail(_hostID, localIP, _msgID);
	VIEW_WRITE_WARNING("Packet Deserialize Failed!! Host:%d, Message:%d, IP:%s", _hostID, _msgID, localIP.c_str());

	NetworkManager::GetInst().Close(_hostID);
	return false;
}

void Service::_UnRegisterHandler(const int& _msgID)
{
	if(nullptr != m_pService)
		m_pService->UnRegisterHandler(_msgID);
}

void Service::_UnRegisterInnerHandler(const int& _protocolID)
{
	if (nullptr != m_pService)
		m_pService->UnRegisterInnerHandler(_protocolID);
}
