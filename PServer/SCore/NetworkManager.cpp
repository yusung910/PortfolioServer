#include "stdafx.hxx"
#include "NetworkManager.h"
#include "NetworkManagerPO.hxx"

NetworkManager::NetworkManager()
{
}


NetworkManager::~NetworkManager()
{
}

void NetworkManager::Init()
{
    m_pManager = new NetworkManagerPO();
}

void NetworkManager::ReserveContext(size_t _reserveContext)
{
    if (nullptr != m_pManager)
        m_pManager->ReserveContext(_reserveContext);
}

size_t NetworkManager::GetContextAllocateCount() const
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextAllocateCount();
}

void NetworkManager::CreateNetwork()
{
    if (nullptr != m_pManager)
        m_pManager->CreateNetwork();
}

void NetworkManager::DestroyNetwork()
{
    if (nullptr != m_pManager)
        m_pManager->DestroyNetwork();
}

//----------------------------------------------------------
//NetworkContext Pool
//----------------------------------------------------------

NetworkContextPO* NetworkManager::AllocateContext()
{
    if(nullptr == m_pManager)
        return nullptr;
    return m_pManager->AllocateContext();
}

void NetworkManager::ReleaseContext(NetworkContextPO* _ctxt)
{
    if (nullptr != m_pManager)
        m_pManager->ReleaseContext(_ctxt);
}

int NetworkManager::GetContextAllocateCount()
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextAllocateCount();
}

int NetworkManager::GetContextUseCount()
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextUseCount();
}

int NetworkManager::GetContextFreeCount()
{
    if (nullptr == m_pManager)
        return 0;

    return m_pManager->GetContextFreeCount();
}
//----------------------------------------------------------
//NetworkContext Pool end
//----------------------------------------------------------

//----------------------------------------------------------
//NetworkStatics begin
//----------------------------------------------------------
void NetworkManager::OnConnect(int _hostID)
{
    if (nullptr != m_pManager)
        m_pManager->OnConnect(_hostID);
}

void NetworkManager::OnDisconnect(int _hostID)
{
    if (nullptr != m_pManager)
        m_pManager->OnDisconnect(_hostID);
}

void NetworkManager::OnSend(const int& _bytes)
{
    if (nullptr != m_pManager)
        m_pManager->OnSend(_bytes);
}

void NetworkManager::OnRecv(const int& _bytes)
{
    if (nullptr != m_pManager)
        m_pManager->OnRecv(_bytes);
}

bool NetworkManager::IsConnected(const int& _hostID)
{
    if(nullptr == m_pManager)
        return false;

    return m_pManager->IsConnected(_hostID);
}
//----------------------------------------------------------
//NetworkStatics end
//----------------------------------------------------------