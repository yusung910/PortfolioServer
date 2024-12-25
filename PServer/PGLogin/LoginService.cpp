#include "pch.h"
#include "LoginService.h"
#include "StringUtil.h"
#include "Clock.h"

#include <NetworkManager.h>

LoginService::LoginService()
{
    RegisterHandler(&LoginService::OnHostConnect);
    RegisterHandler(&LoginService::OnHostClose);
}

LoginService::~LoginService()
{
}

bool LoginService::Start()
{
    return false;
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
    return true;
}

void LoginService::_SendErrorMessage(const int& _hostID, const EErrorMsg& _errorMsg, const EPacketProtocol& _msgID, const bool& _kick)
{
    flatbuffers::FlatBufferBuilder lFbb;
    auto lPacket = CreateSCIntegrationErrorNotification(lFbb, _msgID, (int)_errorMsg);

    if (true == _kick)
    {
        //AddKickReserve(hostid);
    }
}

void LoginService::_KickProcess()
{
    int64_t lNow = Clock::GetTick64();

    AutoLock(m_xKickList);

    for (auto it = m_umKickList.begin(); it != m_umKickList.end(); ++it)
    {
        if (lNow > it->second)
        {
            NetworkManager::GetInst().CloseHost(it->first, "");

            it = m_umKickList.erase(it);
            continue;
        }
        
    }

}
