#include "stdafx.hxx"
#include "ServerApp.h"
#include "ServerAppPO.hxx"

ServerApp::ServerApp()
{
    m_pServerApp = new ServerAppPO();
}

ServerApp::~ServerApp()
{
    SafeDelete(m_pServerApp);
}

void ServerApp::SetListenerInfo(const ListenerInfo& _info)
{
    if (nullptr != m_pServerApp)
        m_pServerApp->SetListenerInfo(_info);
}

void ServerApp::BindEventSync(std::shared_ptr<NetworkEventSync> _eventSync)
{
    if (nullptr != m_pServerApp)
        m_pServerApp->BindEventSync(_eventSync);
}

void ServerApp::RegisterCommand(int _cmd, std::function<void(void)> _func)
{
    if (nullptr != m_pServerApp)
        m_pServerApp->RegisterCommand(_cmd, _func);
}

void ServerApp::ProcessQuit()
{
    if (nullptr != m_pServerApp)
        m_pServerApp->ProcessQuit();
}

void ServerApp::Run()
{
    if (nullptr != m_pServerApp)
        m_pServerApp->Run();
}
