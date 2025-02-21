#include "pch.h"
#include "CPPServerLib.h"

bool CPPServerLib::Compressor::Compress(char* _buffer, const int& _size)
{
    if (nullptr == _buffer)
        return false;

    if (MAX_PACKET_DATA_SIZE < _size
        || _size <= 0)
        return false;

    m_nCompressedSize = LZ4_compress_HC(_buffer, m_cBuffer, _size, MAX_PACKET_DATA_SIZE, LZ4HC_CLEVEL_DEFAULT);

    if (m_nCompressedSize <= 0)
        return false;

    return true;
}
