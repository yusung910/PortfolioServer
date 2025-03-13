
#pragma once

struct MDBNPCSummons;

struct SummonsInfo
{
    int m_nNPCUniqueID = 0;        
    int m_nNPCSummonsCount = 0;
    int m_nResummonsTime = 0;

    const MDBNPCSummons* m_oSummons = nullptr;
};