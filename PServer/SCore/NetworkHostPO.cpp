#include "stdafx.hxx"
#include "NetworkHostPO.hxx"

#include "NetworkSupporterPO.hxx"
#include "NetworkContextPO.hxx"
#include "PacketCompressor.hxx"

#include "NetworkEventSync.h"

#include "Clock.h"

#include <iostream>
#include <Packet.h>

NetworkHostPO::NetworkHostPO()
{
    m_pPacketCompressor = PacketCompressor::New();
}

NetworkHostPO::~NetworkHostPO()
{
    m_pPacketCompressor.reset();
}

void NetworkHostPO::Reset()
{
    Close(ESocketCloseType::Reset);
    m_nHostID = 0;
    m_eHostType = EHostType::None;

    m_pEventSync = nullptr;

    m_lBaseTaskCount = 0;
    m_lSendTaskCount = 0;

    m_nCheckTimeoutMS = 0;
    m_nCheckAliveMS = 0;

    m_sIP.clear();

    {
        AutoLock(m_oSendLock);
        m_oSendWaitingList.clear();
    }

    m_oSendWorkQueue.clear();

    m_bIsClientHost = false;

    if (nullptr != m_pPacketCompressor.get())
        m_pPacketCompressor->Reset();

    m_eLastSocketCloseType = ESocketCloseType::Reset;

    m_bUsePacketRecvCheck = false;
    m_nPacketRecvCheckTick = 0;
    m_nPacketRecvCheckCountter = 0;

    m_nMessageHistoryIdx = 0;
    m_oMessageHistory = {};

    m_nLastPacketTick = 0;
}

void NetworkHostPO::BeginBaseTask()
{
    InterlockedIncrement(&m_lBaseTaskCount);
}

void NetworkHostPO::EndBaseTask (bool _rslt, const ESocketCloseType& _type)
{
    InterlockedDecrement(&m_lBaseTaskCount);

    if (true == _rslt)
        return;

    Close(_type);
}
