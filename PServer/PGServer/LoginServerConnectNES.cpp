#include "pch.h"
#include "LoginServerConnectNES.h"
#include "LoginServerConnectService.h"

void LoginServerConnectNES::OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType)
{
    flatbuffers::FlatBufferBuilder lMsg;
    auto lObj = CreateHostConnect(lMsg
        , lMsg.CreateString(_ip)
        , _port
        , _serverType
        );
    lMsg.Finish(lObj);

    Packet::SharedPtr lPackt = Packet::New();
    lPackt->m_nHostID = _hostID;

    if (true == lPackt->SetPacketData(Host_Connect, lMsg.GetBufferPointer(), lMsg.GetSize()))
        LoginServerConnectService::GetInst().Push(lPackt);
}

void LoginServerConnectNES::OnClose(const int& _hostID)
{
    flatbuffers::FlatBufferBuilder lMsg;
    auto lObj = CreateHostClose(lMsg);
    lMsg.Finish(lObj);

    Packet::SharedPtr lPackt = Packet::New();
    lPackt->m_nHostID = _hostID;

    if (true == lPackt->SetPacketData(Host_Close, lMsg.GetBufferPointer(), lMsg.GetSize()))
        LoginServerConnectService::GetInst().Push(lPackt);
}

void LoginServerConnectNES::OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize)
{
    Packet::SharedPtr lPackt = Packet::New();
    lPackt->m_nHostID = _hostID;

    if (true == lPackt->SetPacketData(_msgID, _msg, _msgSize))
        LoginServerConnectService::GetInst().Push(lPackt);
}

void LoginServerConnectNES::OnListen()
{
    LoginServerConnectService::GetInst().OnListen();
}

void LoginServerConnectNES::OnListenFailed()
{
    LoginServerConnectService::GetInst().OnListenFailed();
}
