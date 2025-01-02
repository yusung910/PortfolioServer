#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include "ServerConfigInfo.h"

class NetworkEventSync;

class ServerAppPO
{
private:
    std::shared_ptr<NetworkEventSync> m_pEventSync = nullptr;
    volatile bool m_bIsRunning = false;
    std::unordered_map<int, std::function<void(void)>> m_umCommandMap;

    ListenerInfo m_oListenerInfo;

public:
    /*!
     *  Constructor.
     */
    ServerAppPO();
    /*!
     *  Destructor.
     */
    virtual ~ServerAppPO();

    void SetListenerInfo(const ListenerInfo& _info);

    void BindEventSync(std::shared_ptr<NetworkEventSync> _eventSync);

    /*!
     *  키보드 입력 값에 해당하는 함수를 등록하는 함수.
     *  이 함수는 보통 생성자에서 실행된다
     *
     *      @param [in] _cmd
     *      @param [in] _func
     */
    void RegisterCommand(int _cmd, std::function<void(void)> _func);
    void ProcessQuit();

    void Run();

private:
    bool _Begin();
    void _Stop();
};

