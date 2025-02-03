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
    std::mutex m_xKickLock;

public:
    LoginService();
    virtual ~LoginService();

    bool Start();
        
    void AddKickReserve(const int& _hostID);
protected:
    bool OnHostConnect(int _hostID, const HostConnect& _msg);
    bool OnHostClose(int _hostID, const HostClose& _msg);

    //Client -> LoginServer
    bool OnCLAuthReq(int _hostID, const CLAuthReq& _msg);

    //UDB -> LoginServer
    bool OnUDBLAuthRes(InnerPacket::SharedPtr _data);

    //Platform -> LoginServer
    bool OnPLAuthLoginRes(InnerPacket::SharedPtr _data);

private:
    void _KickProcess();

    void _Statistics();
    void _UpdateTitle();

    //에러 메세지 전송
    bool _SendErrorMessage(const int& _hostID, const EErrorMsg& _errorMsg, const EPacketProtocol& _msgID, const bool& _kick = false);

    //Login 진행
    bool _AuthLoginProcess(int _hostID, const int& _clientType, const int& _appVer, const ELoginPlatform::Type _pfType, const std::string& _accountToken);
};

