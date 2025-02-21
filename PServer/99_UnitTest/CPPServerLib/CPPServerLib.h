//https://cypsw.tistory.com/entry/C-C-DLL-%EC%9D%84-%EC%82%AC%EC%9A%A9%ED%95%98%EA%B8%B0
#pragma once
#include <Packet.h>

using namespace System;

namespace CPPServerLib {
    public ref class Compressor
    {
    public:
        //압축된 크기
        int m_nCompressedSize = 0;

        char* m_cBuffer = nullptr;

        bool Compress(char* _buffer, const int& _size);
    };
}
