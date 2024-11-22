#include "stdafx.hxx"
#include "ServicePO.hxx"

void ServicePO::Exit()
{
	m_bShouldExit = true;
}

void ServicePO::Push(std::shared_ptr<Packet> _packet)
{
	if (nullptr != _packet)
		m_oOuterPacketQueue.Push(_packet);
}

std::shared_ptr<Packet> ServicePO::Pop()
{
	return m_oOuterPacketQueue.Pop();
}

bool ServicePO::CreateThread()
{
	if (INVALID_HANDLE_VALUE != m_hThread
		&& NULL != m_hThread)
		return false;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, this, 0, nullptr);

	return true;
}

unsigned int ServicePO::Run()
{
	while (false == m_bShouldExit)
	{
		//Timer �۾�
		UpdateTimer();

		//���� ��Ŷ ó��
		_InnerPacketProcess();

		//�߰� �Լ�
		_AdditionalWorkProcess();

		// ��Ŷó��
		_DispatchPacketProcess();
	}

	return 0;
}

size_t ServicePO::GetPacketQueueCount()
{
	return m_oOuterPacketQueue.Count();
}

void ServicePO::AddAdditionalFunc(std::function<void()> _func)
{
	if (nullptr != _func)
		m_oFuncList.push_back(_func);
}

void ServicePO::RegisterHandler(const int& _msgID, std::function<bool(const Packet&)> _func)
{
	if (auto localIter = m_umOuterPacketHandler.find(_msgID); localIter == m_umOuterPacketHandler.end())
		m_umOuterPacketHandler.insert_or_assign(_msgID, _func);
}

void ServicePO::UnRegisterHandler(const int& _msgID)
{
	m_umOuterPacketHandler.erase(_msgID);
}

void ServicePO::InnerPacketPush(InnerPacket::SharedPtr _data)
{
	if (nullptr == _data.get())
		return;

	m_oInnerPacketQueue.Push(_data);
}

void ServicePO::RegisterInnerHandler(const int& _protocolID, const std::function<bool(InnerPacket::SharedPtr)>& _func)
{
	m_umInnerPacketHandler.insert_or_assign(_protocolID, _func);
}

void ServicePO::UnRegisterInnerHandler(const int& _protocolID)
{
	m_umInnerPacketHandler.erase(_protocolID);
}

bool ServicePO::_DispatchPacket(const Packet& _packet)
{
	if (auto localIter = m_umOuterPacketHandler.find(_packet.GetMessageID()); localIter != m_umOuterPacketHandler.end())
	{
		if (nullptr != localIter->second)
			return localIter->second(_packet);
	}
	return false;
}

void ServicePO::_DispatchPacketProcess()
{
	m_oOuterPacketWorkList.clear();
	m_oOuterPacketQueue.Swap(m_oOuterPacketWorkList);

	for (auto& localPacket : m_oOuterPacketWorkList)
	{
		if (false == _DispatchPacket(*localPacket))
		{
			//Dispatch Packet ���� �߻��� ���� �α� �ۼ� �κ�
		}

		localPacket.reset(); // �Ҹ�ó��
	}

	if (true == m_oOuterPacketWorkList.empty())
		::Sleep(1);
}

bool ServicePO::_DispatchInnerPacket(InnerPacket::SharedPtr& _packet)
{
	if (nullptr == _packet)
		return false;

	if (auto localIter = m_umInnerPacketHandler.find(_packet->m_nProtocol); localIter != m_umInnerPacketHandler.end())
	{
		if (nullptr != localIter->second)
			return localIter->second(_packet);
	}

	return false;
}

void ServicePO::_InnerPacketProcess()
{
	m_oInnerPacketWorkList.clear();
	m_oInnerPacketQueue.Swap(m_oInnerPacketWorkList);

	for (auto& localData : m_oInnerPacketWorkList)
	{
		if (auto localIter = m_umInnerPacketHandler.find(localData->m_nProtocol); localIter != m_umInnerPacketHandler.end())
		{
			if (nullptr != localIter->second)
				localIter->second(localData);
		}
	}
}

void ServicePO::_AdditionalWorkProcess()
{
	for (auto localIter = m_oFuncList.begin(); localIter != m_oFuncList.end(); ++localIter)
	{
		if (nullptr != *localIter)
			(*localIter)();
	}
}

unsigned int __stdcall ServicePO::ThreadFunc(void* _pSelf)
{
	if (nullptr == _pSelf)
		return 0;

	return static_cast<ServicePO*>(_pSelf)->Run();
}
