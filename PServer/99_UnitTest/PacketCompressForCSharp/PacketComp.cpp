#include "PacketCompPrivate.h"
#include "PacketComp.h"

PacketComp::PacketComp()
{
}

void PacketComp::Reset()
{
}

bool PacketComp::Compress(char* _pBuffer, const int& _size)
{
    if (nullptr == _pBuffer)
        return false;

    if (MAX_PACKET_DATA_SIZE < _size
        || _size <= 0)
        return false;

    m_nCompressedSize = LZ4_compress_HC(_pBuffer, m_cCompressBuffer, _size, MAX_PACKET_DATA_SIZE, LZ4HC_CLEVEL_DEFAULT);

    if (m_nCompressedSize <= 0)
        return false;

    return true;
}

bool PacketComp::Decompress(char* _pBuffer, const int& _size)
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
