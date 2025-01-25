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
	if (auto lIter = m_umOuterPacketHandler.find(_msgID); lIter == m_umOuterPacketHandler.end())
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
	if (auto lIter = m_umOuterPacketHandler.find(_packet.GetMessageID()); lIter != m_umOuterPacketHandler.end())
	{
		if (nullptr != lIter->second)
			return lIter->second(_packet);
	}
	return false;
}

void ServicePO::_DispatchPacketProcess()
{
	m_oOuterPacketWorkList.clear();
	m_oOuterPacketQueue.Swap(m_oOuterPacketWorkList);

	for (auto& lPacket : m_oOuterPacketWorkList)
	{
		if (false == _DispatchPacket(*lPacket))
		{
			//Dispatch Packet ���� �߻��� ���� �α� �ۼ� �κ�
			VIEW_WRITE_ERROR("DispatchPacket Fail! is not Packet Function(), MessageID: %d", lPacket->GetMessageID());
		}

		lPacket.reset(); // �Ҹ�ó��
	}

	if (true == m_oOuterPacketWorkList.empty())
		::Sleep(1);
}

bool ServicePO::_DispatchInnerPacket(InnerPacket::SharedPtr& _packet)
{
	if (nullptr == _packet)
		return false;

	if (auto lIter = m_umInnerPacketHandler.find(_packet->m_nProtocol); lIter != m_umInnerPacketHandler.end())
	{
		if (nullptr != lIter->second)
			return lIter->second(_packet);
	}

	return false;
}

void ServicePO::_InnerPacketProcess()
{
	m_oInnerPacketWorkList.clear();
	m_oInnerPacketQueue.Swap(m_oInnerPacketWorkList);

	for (auto& lData : m_oInnerPacketWorkList)
	{
		if (auto lIter = m_umInnerPacketHandler.find(lData->m_nProtocol); lIter != m_umInnerPacketHandler.end())
		{
			if (nullptr != lIter->second)
				lIter->second(lData);
		}
	}
}

void ServicePO::_AdditionalWorkProcess()
{
	for (auto lIter = m_oFuncList.begin(); lIter != m_oFuncList.end(); ++lIter)
	{
		if (nullptr != *lIter)
			(*lIter)();
	}
}

unsigned int __stdcall ServicePO::ThreadFunc(void* _pSelf)
{
	if (nullptr == _pSelf)
		return 0;

	return static_cast<ServicePO*>(_pSelf)->Run();
}
