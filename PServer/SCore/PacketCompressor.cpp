#include "stdafx.hxx"
#include "PacketCompressor.hxx"

PacketCompressor::PacketCompressor()
    :PacketCompressor(ObjectTag())
{
}

PacketCompressor::PacketCompressor(ObjectTag)
{
}

void PacketCompressor::Reset()
{
    m_nCompressedSize = 0;
    memset(m_cCompressBuffer, 0, sizeof(m_cCompressBuffer));
}

bool PacketCompressor::Compress(char* _pBuffer, const int& _size)
{
    if (nullptr == _pBuffer)
        return false;

    if (MAX_PACKET_DATA_SIZE < _size 
        || _size <= 0)
        return false;

    m_nCompressedSize = LZ4_compress_default(_pBuffer, m_cCompressBuffer, _size, MAX_PACKET_DATA_SIZE);

    if (m_nCompressedSize <= 0)
        return false;

    return true;
}

bool PacketCompressor::Decompress(char* _pBuffer, const int& _size)
{
    if (nullptr == _pBuffer)
        return false;

    if (MAX_PACKET_DATA_SIZE < _size 
        || _size <= 0)
        return false;

    m_nCompressedSize = LZ4_decompress_safe(_pBuffer, m_cCompressBuffer, _size, MAX_PACKET_BINARY_SIZE);

    if (m_nCompressedSize <= 0)
        return false;

    return true;
}

bool PacketCompressor::CompressPayload(char* _pBuffer, const int& _payloadSize)
{
    if (nullptr == _pBuffer)
        return false;

    if (MAX_PACKET_BINARY_SIZE < _payloadSize 
        || _payloadSize <= PACKET_HEADER_SIZE)
        return false;

    m_nCompressedSize = LZ4_compress_default(_pBuffer + PACKET_HEADER_SIZE, m_cCompressBuffer + PACKET_HEADER_SIZE, _payloadSize - PACKET_HEADER_SIZE, MAX_PACKET_DATA_SIZE);

    if (m_nCompressedSize <= 0)
        return false;

    *((int*)m_cCompressBuffer) = m_nCompressedSize;

    memcpy_s(m_cCompressBuffer + sizeof(int), sizeof(int), _pBuffer + sizeof(int), sizeof(int));
    return true;
}

bool PacketCompressor::DecompressPayload(char* _pBuffer, const int& _payloadSize)
{
    if (nullptr == _pBuffer)
        return false;

    if (MAX_PACKET_BINARY_SIZE < _payloadSize
        || _payloadSize <= PACKET_HEADER_SIZE)
        return false;

    m_nCompressedSize = LZ4_decompress_safe(_pBuffer + PACKET_HEADER_SIZE, m_cCompressBuffer + PACKET_HEADER_SIZE, _payloadSize - PACKET_HEADER_SIZE, MAX_PACKET_DATA_SIZE);

    if (m_nCompressedSize <= 0)
        return false;

    *((int*)m_cCompressBuffer) = m_nCompressedSize;

    memcpy_s(m_cCompressBuffer + sizeof(int), sizeof(int), _pBuffer + sizeof(int), sizeof(int));
    return true;;
}
