#include "pch.h"
#include "LoginDBService.h"

LoginDBService::LoginDBService()
{

    RegisterHandler(EPacketProtocol::LUDB_AuthReq, &LoginDBService::OnLULoginReq);
}

LoginDBService::~LoginDBService()
{
}

bool LoginDBService::OnLULoginReq(std::shared_ptr<InnerPacket> _data)
{
    CheckSession();


    return false;
}
