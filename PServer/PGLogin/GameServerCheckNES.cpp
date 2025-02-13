#include "pch.h"
#include "GameServerCheckNES.h"
#include "GameServerCheckService.h"

void GameServerCheckNES::OnConnect(const int& _hostID, const std::string& _ip, const int& _port, [[maybe_unused]] const int& _serverType)
{
    flatbuffers::FlatBufferBuilder lMsg;
    auto lObj = CreateHostConnect(lMsg, lMsg.CreateString(_ip), _port, _serverType);
    lMsg.Finish(lObj);

    Packet::SharedPtr lPacket = Packet::New();
    lPacket->m_nHostID = _hostID;

    if (true == lPacket->SetPacketData(Host_Connect, lMsg.GetBufferPointer(), lMsg.GetSize()))
        GameServerCheckService::GetInst().Push(lPacket);
}

void GameServerCheckNES::OnClose(const int& _hostID)
{
    VIEW_INFO("Client on Closed");
    flatbuffers::FlatBufferBuilder lMsg;
    auto lObj = CreateHostClose(lMsg);
    lMsg.Finish(lObj);

    Packet::SharedPtr lPacket = Packet::New();
    lPacket->m_nHostID = _hostID;

    if (true == lPacket->SetPacketData(Host_Connect, lMsg.GetBufferPointer(), lMsg.GetSize()))
        GameServerCheckService::GetInst().Push(lPacket);

}

void GameServerCheckNES::OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize)
{
    Packet::SharedPtr lPacket = Packet::New();
    lPacket->m_nHostID = _hostID;

    if (true == lPacket->SetPacketData(_msgID, _msg, _msgSize))
        GameServerCheckService::GetInst().Push(lPacket);
}
