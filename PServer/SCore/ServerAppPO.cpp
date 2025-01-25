#include "stdafx.hxx"
#include "ServerAppPO.hxx"
#include "NetworkManager.h"
#include <conio.h>
#include "NetworkEventSync.h"

ServerAppPO::ServerAppPO()
{
#ifdef DEV_TEST
    RegisterCommand('q', std::bind(&ServerAppPO::ProcessQuit, this));
#endif // DEV_TEST

}

ServerAppPO::~ServerAppPO()
{
}

void ServerAppPO::SetListenerInfo(const ListenerInfo& _info)
{
    m_oListenerInfo = _info;
}

void ServerAppPO::BindEventSync(std::shared_ptr<NetworkEventSync> _eventSync)
{
    m_pEventSync = _eventSync;
}

void ServerAppPO::RegisterCommand(int _cmd, std::function<void(void)> _func)
{
    auto iter = m_umCommandMap.find(_cmd);
    if (iter != m_umCommandMap.end())
    {
        VIEW_WARNING(L"ServerAppPO::RegisterCommand() Failed - _cmd: %d is Duplicated _cmd", _cmd);
        return;
    }

    m_umCommandMap[_cmd] = _func;
}

void ServerAppPO::ProcessQuit()
{
    m_bIsRunning = false;
}

void ServerAppPO::Run()
{
    if (true == _Begin())
    {
        auto lName = StringUtil::ToWideChar(m_oListenerInfo.m_sServiceName);
        VIEW_WRITE_INFO(L"Server:%s Memo:server_waiting_for_keyboard", lName.c_str());
        m_bIsRunning = true;
        while (true == m_bIsRunning)
        {
            if (_kbhit())
            {
                auto lIter = m_umCommandMap.find(_getch());
                if (lIter != m_umCommandMap.end())
                {
                    (lIter->second)();
                }
            }

            Sleep(10);
        }
    }
    _Stop();
}

bool ServerAppPO::_Begin()
{
    //네트워크 생성
    NetworkManager::GetInst().CreateNetwork();

    //서버 별 NetworkEventSync가 생성 되지 않았을 경우 중단
    if (nullptr == m_pEventSync)
    {
        VIEW_WRITE_ERROR(L"Server:%s _Begin() Failed - m_pEventSync is nullptr", StringUtil::ToWideChar(m_oListenerInfo.m_sServiceName).c_str());
        return false;
    }

    m_pEventSync->SetTimeoutMS(m_oListenerInfo.m_nTimeoutMS);

    auto lServiceName = StringUtil::ToWideChar(m_oListenerInfo.m_sServiceName);
    auto lAddress = m_oListenerInfo.m_sBindAddress;
    auto lPort = m_oListenerInfo.m_nBindPort;

    VIEW_SYSTEM_FORCE(L"Server:%s, Host:%s, Port:%d", lServiceName.c_str(), StringUtil::ToWideChar(lAddress).c_str(), lPort);

    if (NetworkManager::GetInst().Listen(m_pEventSync.get(), lAddress, lPort) == false)
    {
        VIEW_SYSTEM_FORCE(L"Server:%s, _Begin() Failed - Network Listen is false", lServiceName.c_str());
    }

    VIEW_SYSTEM_FORCE(L"Server:%s, is Started", lServiceName.c_str());

    return true;
}

void ServerAppPO::_Stop()
{
    NetworkManager::GetInst().DestroyNetwork();
}
