#pragma once
#include <RefSingleton.h>
#include <Service.h>
#include <mutex>
#include <GlobalEnum.h>

constexpr int64_t LOGIN_SERVICE_KICK_DELAY_MS = 1000;
constexpr int64_t KICK_RESERVE_DELAY_MS = 3000;


class LoginService : public Service, public RefSingleton<LoginService>
{
private:

    std::unordered_map<int, int64_t> m_umKickList;
    std::mutex m_xKickList;

public:
    LoginService();
    virtual ~LoginService();

    bool Start();
        

protected:
    bool OnHostConnect(int _hostID, const HostConnect& _msg);
    bool OnHostClose(int _hostID, const HostConnect& _msg);


};

