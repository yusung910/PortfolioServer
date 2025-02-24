#pragma once
#include <Packet.h>

class PacketComp
{
public:
    //����� ������
    int m_nCompressedSize = 0;

    //����� �����Ͱ� �� ����
    char m_cCompressBuffer[MAX_PACKET_BINARY_SIZE] = { 0, };

public:
    PacketComp();
    virtual ~PacketComp() = default;

    virtual void Reset();

    bool Compress(char* _pBuffer, const int& _size);
    bool Decompress(char* _pBuffer, const int& _size);
};

