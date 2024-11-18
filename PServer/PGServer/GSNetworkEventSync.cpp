#include "pch.h"
#include "GSNetworkEventSync.h"
#include "GameService.h"

void GSNetworkEventSync::OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType)
{
	flatbuffers::FlatBufferBuilder localMSG;
	auto localObj = CreateHostConnect(localMSG
									, localMSG.CreateString(_ip)
									, _port
									, _serverType);

	localMSG.Finish(localObj);

	Packet::SharedPtr localPacket = Packet::New();
	localPacket->HostID = _hostID;

	if(true == localPacket->SetPacketData(Host_Connect, localMSG.GetBufferPointer(), localMSG.GetSize()))
		GameService::GetInst().Push(localPacket);
}

void GSNetworkEventSync::OnClose(const int& _hostID)
{
    flatbuffers::FlatBufferBuilder localMSG;
    auto localObj = CreateHostClose(localMSG);
    localMSG.Finish(localObj);


    Packet::SharedPtr localPacket = Packet::New();
    localPacket->HostID = _hostID;

    if (true == localPacket->SetPacketData(Host_Connect, localMSG.GetBufferPointer(), localMSG.GetSize()))
        GameService::GetInst().Push(localPacket);

}

void GSNetworkEventSync::OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize)
{
}
