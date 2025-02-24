#pragma once
#include <Packet.h>

class PacketComp
{
public:
    //압축된 싸이즈
    int m_nCompressedSize = 0;

    //압축된 데이터가 들어갈 변수
    char m_cCompressBuffer[MAX_PACKET_BINARY_SIZE] = { 0, };

public:
    PacketComp();
    virtual ~PacketComp() = default;

    virtual void Reset();

    bool Compress(char* _pBuffer, const int& _size);
    bool Decompress(char* _pBuffer, const int& _size);
};

