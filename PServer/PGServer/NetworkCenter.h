#pragma once
#include "GameService.h"


//to Game Server
template <typename T,
    typename = typename std::enable_if<std::is_base_of<InnerPacket, T>::value>::type>
inline void SendToGame(const int& _hostID, const EPacketProtocol& _protocolID, T* _msg)
{
    if (nullptr == _msg)
        return; 

    InnerPacket::SharedPtr lPackt = InnerPacket::New();
    lPackt->m_nHostID = _hostID;
    lPackt->m_nProtocol = (int)_protocolID;
    lPackt->m_pData = _msg;

    GameService::GetInst().InnerPacketPush(lPackt);
}