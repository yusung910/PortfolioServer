#include "pch.h"
#include "LoginService.h"
#include "StringUtil.h"

#include <NetworkManager.h>

LoginService::LoginService()
{
    RegisterHandler(&LoginService::OnHostClose);
    RegisterHandler(&LoginService::OnHostClose);
}

LoginService::~LoginService()
{
}

bool LoginService::OnHostConnect(int _hostID, const HostConnect& _msg)
{
    VIEW_DEBUG(L"Host(%d) Connected::(%s : %d)", _hostID, StringUtil::ToWideChar(_msg.peerip()->data()).c_str(), _msg.peerport());
    NetworkManager::GetInst().OnConnect(_hostID);
    NetworkManager::GetInst().SetClientHostMode(_hostID, true);

    return true;
}

bool LoginService::OnHostClose(int _hostID, const HostConnect& _msg)
{
    //auto pUserData = LoginUserManager::GetInstance().Find(hostid);
    //if (nullptr != pUserData)
    //{
    //    spAccountUpdateConnServerIDClear* pReq = new spAccountUpdateConnServerIDClear;
    //    pReq->mAccountUniqueID = pUserData->AccountUniqueID;

    //    SendToUDBNew(hostid, EPacketProtocol::LUDB_UpdateConnServerIDClear, pReq);
    //}

    //LoginUserManager::GetInstance().Remove(hostid);
    VIEW_DEBUG(L"Host(%d) Disconnected.", _hostID);
    NetworkManager::GetInst().OnDisconnect(_hostID);
    return false;
}
