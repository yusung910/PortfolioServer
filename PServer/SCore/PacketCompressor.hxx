/**
 *  @file PacketCompressor.hxx
 *  @author YS
 *  @date 2024-10-15
 *  @project SCore
 *
 *  ��Ʈ��ũ ��ſ� ���Ǵ� ��Ŷ �����͸� �����ϴµ� ����ϴ� Ŭ����
 */
#pragma once
#include <Packet.h>
#include <Object.h>

class PacketCompressor : public Object<PacketCompressor>
{
public:
    //����� ������
    int m_nCompressedSize = 0;
    
    //����� �����Ͱ� �� ����
    char m_cCompressBuffer[MAX_PACKET_BINARY_SIZE] = { 0, };

public:
    PacketCompressor();
    PacketCompressor(ObjectTag);
    virtual ~PacketCompressor() = default;

    virtual void Reset();

    bool Compress(char* _pBuffer, const int& _size);
    bool Decompress(char* _pBuffer, const int& _size);

    bool CompressPayload(char* _pBuffer, const int& _payloadSize);
    bool DecompressPayload(char* _pBuffer, const int& _payloadSize);
};

