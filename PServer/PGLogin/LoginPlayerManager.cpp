#include "pch.h"
#include "LoginPlayerManager.h"
#include <NetworkManager.h>
LoginPlayerManager::LoginPlayerManager()
{
}

LoginPlayerManager::~LoginPlayerManager()
{
    //클래스가 소멸 할 때 굳이..??
    //AutoLock(m_xHostLock);

    m_umPlayerList.clear();
    m_umOTPList.clear();
}

LoginPlayer* LoginPlayerManager::Add(int& _hostID, const CLAuthReq& _msg)
{
    AutoLock(m_xHostLock);

    auto lIt = m_umPlayerList.find(_hostID);
    if (lIt != m_umPlayerList.end())
        return nullptr;

    LoginPlayer::SharedPtr lAddPlayer = LoginPlayer::New();
    lAddPlayer->m_nHostID = _hostID;
    lAddPlayer->m_eClientType = (EClient::Type)_msg.ClientType();

    lAddPlayer->m_nAppVersion = _msg.AppVer();
    lAddPlayer->m_eStoreType = (EStore::Type)_msg.StoreType();

    lAddPlayer->m_sAuthKey = _msg.UniqueKey()->c_str();

    m_umPlayerList.insert_or_assign(_hostID, lAddPlayer);

    return lAddPlayer.get();
}

void LoginPlayerManager::Remove(const int& _hostID)
{
    AutoLock(m_xHostLock);

    auto lIt = m_umPlayerList.find(_hostID);
    if (lIt != m_umPlayerList.end())
        m_umPlayerList.erase(lIt);
}

LoginPlayer* LoginPlayerManager::Find(const int& _hostID)
{
    AutoLock(m_xHostLock);

    auto lIt = m_umPlayerList.find(_hostID);
    if (lIt != m_umPlayerList.end())
        return lIt->second.get();

    return nullptr;
}

LoginPlayer* LoginPlayerManager::FindByAccountSeq(const int& _accountSeq)
{
    AutoLock(m_xHostLock);

    for (auto lIt = m_umPlayerList.begin(); lIt != m_umPlayerList.end(); ++lIt)
    {
        if (nullptr == lIt->second)
            continue;
        if (lIt->second.get()->m_nAccountSeq == _accountSeq)
            return lIt->second.get();
    }
    return nullptr;
}

size_t LoginPlayerManager::GetConnectPlayerCount()
{
    AutoLock(m_xHostLock);
    return m_umPlayerList.size();
}

void LoginPlayerManager::GetOTP(const int& _accountSeq, int& _otp)
{
    AutoLock(m_xHostLock);
    auto lIt = m_umOTPList.find(_accountSeq);
    if (lIt != m_umOTPList.end())
        _otp = lIt->second;

}

void LoginPlayerManager::SetOTP(const int& _accountSeq, int& _otp)
{
    AutoLock(m_xHostLock);
    m_umOTPList.insert_or_assign(_accountSeq, _otp);
}

bool LoginPlayerManager::GetPlayerData(const int& _accountSeq, int& _hostID, int& _ServerID, std::string& _platformID, std::string& _returnToken)
{
    AutoLock(m_xHostLock);
    for (auto lIt = m_umPlayerList.begin(); lIt != m_umPlayerList.end(); ++lIt)
    {
        if (nullptr == lIt->second)
            continue;

        if (_accountSeq == lIt->second->m_nAccountSeq)
        {
            _hostID = lIt->second->m_nHostID;
            _ServerID = lIt->second->m_nSelectedServerID;
            _platformID = lIt->second->m_sPlatformID;
            _returnToken = lIt->second->m_sReturnToken;

            return true;
        }

    }
    return false;
}

bool LoginPlayerManager::GetPlayerDataForDeque(const int& _hostID, int& _accountSeq, int& _ServerID, int& _otp, EStore::Type& _storeType)
{
    AutoLock(m_xHostLock);
    auto lIt = m_umPlayerList.find(_hostID);
    if (lIt == m_umPlayerList.end())
        return false;

    if (nullptr == lIt->second)
        return false;

    _accountSeq = lIt->second->m_nAccountSeq;
    _ServerID = lIt->second->m_nSelectedServerID;
    _otp = lIt->second->m_nOTP;
    _storeType = lIt->second->m_eStoreType;

    return true;
}

bool LoginPlayerManager::SetServerID(const int& _hostID, const int& _serverID)
{
    AutoLock(m_xHostLock);
    return false;
}

void LoginPlayerManager::Broadcast(const int& _msgID, flatbuffers::FlatBufferBuilder& _fbb)
{
    std::unordered_map<int, LoginPlayer::SharedPtr> lHostList;

    AutoLock(m_xHostLock);
    lHostList = m_umPlayerList;

    for (const auto& lIt : lHostList)
    {
        const int& lHostID = lIt.first;
        SendPacket(lHostID, _msgID, _fbb);
    }
}

void LoginPlayerManager::Broadcast(const int& _msgID, void* _msg, const size_t& _msgSize)
{
    std::unordered_map<int, LoginPlayer::SharedPtr> lHostList;

    AutoLock(m_xHostLock);
    lHostList = m_umPlayerList;

    for (const auto& lIt : lHostList)
    {
        const int& lHostID = lIt.first;
        SendPacket(lHostID, _msgID, _msg, _msgSize);
    }
}

void LoginPlayerManager::SendPacket(const int& _hostID, const int& _msgID, flatbuffers::FlatBufferBuilder& _fbb)
{
    SendPacket(_hostID, _msgID, _fbb.GetBufferPointer(), _fbb.GetSize());
}

void LoginPlayerManager::SendPacket(const int& _hostID, const int& _msgID, void* _msg, const size_t& _msgSize)
{
    AutoLock(m_xHostLock);
    if (nullptr == _msg)
        return;

    if (m_umPlayerList.find(_hostID) == m_umPlayerList.end())
        return;

    Packet::SharedPtr lPacket = Packet::New();
    lPacket->SetPacketData(_msgID, _msg, _msgSize);
    NetworkManager::GetInst().Send(_hostID, lPacket);

}
