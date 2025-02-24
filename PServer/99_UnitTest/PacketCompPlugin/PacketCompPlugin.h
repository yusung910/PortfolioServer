#pragma once

#include "../PacketCompressForCSharp/PacketComp.h"

using namespace System;

namespace PacketCompPlugin {
	public ref class Compressor
	{
		// TODO: 여기에 이 클래스에 대한 메서드를 추가합니다.
    protected:
        PacketComp* m_oCompressor;

    public:
        Compressor();
        virtual ~Compressor();

        bool Compress(char* _pBuffer, const int& _size);
        bool Decompress(char* _pBuffer, const int& _size);
	};
}
