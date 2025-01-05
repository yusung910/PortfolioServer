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
    bool OnLULoginReq(std::shared_ptr<InnerPacket> _data);
};

