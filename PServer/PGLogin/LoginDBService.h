#pragma once
#include <RefSingleton.h>
#include <DBService.h>
class LoginDBService : public DBService
{
private:
    int m_nCurrentServerID = 1;

public:
    LoginDBService();
    virtual ~LoginDBService();

private:
    bool _OnLUDBLoginReq(std::shared_ptr<InnerPacket> _data);
    bool _OnLUDBConnectServerIDClear(std::shared_ptr<InnerPacket> _data);

    bool _ConnectServerIDClear(int _serverID);


};

