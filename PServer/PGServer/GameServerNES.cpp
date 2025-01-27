#include "pch.h"
#include "GameServerNES.h"
#include "GameService.h"
#include "LoggingMacro.h"

void GameServerNES::OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType)
{
	flatbuffers::FlatBufferBuilder lMSG;
	auto lObj = CreateHostConnect(lMSG
									, lMSG.CreateString(_ip)
									, _port
									, _serverType);

	lMSG.Finish(lObj);

	Packet::SharedPtr lPacket = Packet::New();
	lPacket->HostID = _hostID;

	if(true == lPacket->SetPacketData(Host_Connect, lMSG.GetBufferPointer(), lMSG.GetSize()))
		GameService::GetInst().Push(lPacket);
}

void GameServerNES::OnClose(const int& _hostID)
{
    flatbuffers::FlatBufferBuilder lMSG;
    auto lObj = CreateHostClose(lMSG);
    lMSG.Finish(lObj);


    Packet::SharedPtr lPacket = Packet::New();
    lPacket->HostID = _hostID;

    if (true == lPacket->SetPacketData(Host_Connect, lMSG.GetBufferPointer(), lMSG.GetSize()))
        GameService::GetInst().Push(lPacket);

}

void GameServerNES::OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize)
{
	Packet::SharedPtr lMSG = Packet::New();
	lMSG->SetPacketData(_msgID, _msg, _msgSize);
	lMSG->HostID = _hostID;

    //

	GameService::GetInst().Push(lMSG);
}
