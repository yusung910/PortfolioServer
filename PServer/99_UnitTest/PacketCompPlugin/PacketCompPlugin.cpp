#include "pch.h"

#include "PacketCompPlugin.h"

namespace PacketCompPlugin
{
    PacketCompPlugin::Compressor::Compressor() :
        m_oCompressor(new PacketComp)
    {

    }

    PacketCompPlugin::Compressor::~Compressor()
    {
        if (nullptr != m_oCompressor)
            delete m_oCompressor;

    }

    bool PacketCompPlugin::Compressor::Compress(char* _pBuffer, const int& _size)
    {
        if (nullptr != m_oCompressor)
            return m_oCompressor->Compress(_pBuffer, _size);
        return false;
    }

    bool PacketCompPlugin::Compressor::Decompress(char* _pBuffer, const int& _size)
    {
        if (nullptr != m_oCompressor)
            return m_oCompressor->Decompress(_pBuffer, _size);
        return false;
    }
}