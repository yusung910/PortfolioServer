/**
 *  @file PacketCompressor.hxx
 *  @author YS
 *  @date 2024-10-15
 *  @project SCore
 *
 *  네트워크 통신에 사용되는 패킷 데이터를 압축하는데 사용하는 클래스
 */
#pragma once
#include <Packet.h>
#include <Object.h>

class PacketCompressor : public Object<PacketCompressor>
{
public:
    //압축된 싸이즈
    int m_nCompressedSize = 0;
    
    //압축된 데이터가 들어갈 변수
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

