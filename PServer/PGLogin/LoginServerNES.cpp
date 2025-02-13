#include "pch.h"
#include "LoginServerNES.h"
#include "LoginService.h"

void LoginServerNES::OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType)
{
    flatbuffers::FlatBufferBuilder lMSG;
    auto lObj = CreateHostConnect(lMSG
        , lMSG.CreateString(_ip)
        , _port
        , _serverType);

    lMSG.Finish(lObj);

    Packet::SharedPtr lPacket = Packet::New();
    lPacket->m_nHostID= _hostID;

    if (true == lPacket->SetPacketData(Host_Connect, lMSG.GetBufferPointer(), lMSG.GetSize()))
        LoginService::GetInst().Push(lPacket);
}

void LoginServerNES::OnClose(const int& _hostID)
{
    flatbuffers::FlatBufferBuilder lMSG;
    auto lObj = CreateHostClose(lMSG);

    lMSG.Finish(lObj);

    Packet::SharedPtr lPacket = Packet::New();
    lPacket->m_nHostID = _hostID;

    if (true == lPacket->SetPacketData(Host_Close, lMSG.GetBufferPointer(), lMSG.GetSize()))
        LoginService::GetInst().Push(lPacket);
}

void LoginServerNES::OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize)
{
    Packet::SharedPtr lMSG = Packet::New();

    lMSG->m_nHostID = _hostID;

    if(true == lMSG->SetPacketData(_msgID, _msg, _msgSize))
        LoginService::GetInst().Push(lMSG);
}
