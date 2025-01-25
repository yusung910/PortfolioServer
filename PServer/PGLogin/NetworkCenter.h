#pragma once
//Inner Packet 통신을 위한 inline 함수

#include "LoginDBLoadBalancer.h"
#include "LoginService.h"
#include "OPFService.h"

#include <InnerPacket.h>

// to UDB
template <typename T>
inline void SendToUDB(const int& _hostID, const EPacketProtocol& _msgID, T* _msg)
{
    if (nullptr == _msg)
        return;

    InnerPacket::SharedPtr lPacket = InnerPacket::New();
    lPacket->m_nHostID = _hostID;
    lPacket->m_nProtocol = (int)_msgID;
    lPacket->m_pData = _msg;

    LoginDBLoadBalancer::GetInst().PushDirect(lPacket);
}



inline void SendToUDB(const EPacketProtocol& _msgID, InnerPacket::SharedPtr _packet)
{
    if (nullptr == _packet.get())
        return;

    _packet->m_nProtocol = (int)_msgID;

    LoginDBLoadBalancer::GetInst().PushDirect(_packet);
}


template <typename T>
inline void SendToUDB(const EPacketProtocol& _msgID, InnerPacket::SharedPtr _packet, T* _msg)
{
    if (nullptr == _packet.get())
        return;

    _packet->m_nProtocol = (int)_msgID;
    SafeDelete(_packet->m_pData);
    _packet->m_pData = _msg;

    LoginDBLoadBalancer::GetInst().PushDirect(_packet);
}


// to LoginService
template <typename T>
inline void SendToLoginService(const int& _hostID, const EPacketProtocol& _msgID, T* _msg)
{
    if (nullptr == _msg)
        return;

    InnerPacket::SharedPtr lPacket = InnerPacket::New();
    lPacket->m_nHostID = _hostID;
    lPacket->m_nProtocol = _msgID;
    lPacket->m_pData = _msg;

    LoginService::GetInst().InnerPacketPush(lPacket);
}

inline void SendToLoginService(const EPacketProtocol& _msgID, InnerPacket::SharedPtr _packet)
{
    if (nullptr == _packet.get())
        return;

    _packet->m_nProtocol = (int) _msgID;
    LoginService::GetInst().InnerPacketPush(_packet);
}

template<typename T>
inline void SendToLoginService(const EPacketProtocol& _msgID, InnerPacket::SharedPtr _packet, T* _msg)
{
    if (nullptr == _packet)
        return;

    _packet->m_nProtocol = _msgID;
    SafeDelete(_packet->m_pData);
    _packet->m_pData = _msg;

    LoginService::GetInst().InnerPacketPush(_packet);
}

//send to Platform service
template <typename T>
inline void SendToPlatform(const int& _hostID, const EPacketProtocol& _msgID, T* _msg)
{
    if (nullptr == _msg)
        return;

    InnerPacket::SharedPtr lPacket = InnerPacket::New();
    lPacket->m_nHostID = _hostID;
    lPacket->m_nProtocol = _msgID;
    lPacket->m_pData = _msg;

    OPFService::GetInst().InnerPacketPush(lPacket);
}


//send to PF