#include "stdafx.hxx"
#include "LZ4Compressor.h"
#include "DefineMacro.h"

#include <lz4.h>

int LZ4Compressor::Compress(char* pSrcDstData_io, const int& nSrcSize, const int& nSrcBufferMaxSize, char* pTempBuffer, const int& nTempBufferSize)
{
    return Compress(pSrcDstData_io, nSrcSize, pSrcDstData_io, nSrcBufferMaxSize, pTempBuffer, nTempBufferSize);
}

int LZ4Compressor::Decompress(char* pSrcDstData_io, const int& nSrcSize, const int& nSrcBufferMaxSize, char* pTempBuffer, const int& nTempBufferSize)
{
    return Decompress(pSrcDstData_io, nSrcSize, pSrcDstData_io, nSrcBufferMaxSize, pTempBuffer, nTempBufferSize);
}

int LZ4Compressor::Compress(const char* pSrcData, const int& nSrcSize, char* pDstBuffer, const int& nDstBufferSize, char* pTempBuffer, const int& nTempBufferSize)
{
    if (nullptr == pSrcData
        || nullptr == pDstBuffer
        || nullptr == pTempBuffer
        || nSrcSize <= 0
        || nDstBufferSize <= 0
        || nTempBufferSize <= 0)
        return 0;

    int nRet = LZ4_compress_default(pSrcData, pTempBuffer, nSrcSize, static_cast<int>(nTempBufferSize));
    if (nRet < nSrcSize)
    {
        // 압축 성공!... 이지만
        if (nRet > nDstBufferSize)
            nRet = 0;   // 대상 버퍼 사이즈가 작아
        else
            memcpy_s(pDstBuffer, static_cast<rsize_t>(nDstBufferSize), pTempBuffer, static_cast<rsize_t>(nRet));
    }
    else
    {
        nRet = 0; // 압축 실패
    }

    return nRet;    // 압축된 크기
}

int LZ4Compressor::Decompress(const char* pSrcData, const int& nSrcSize, char* pDstBuffer, const int& nDstBufferSize, char* pTempBuffer, const int& nTempBufferSize)
{
    if (nullptr == pSrcData
        || nullptr == pDstBuffer
        || nullptr == pTempBuffer
        || nSrcSize <= 0
        || nDstBufferSize <= 0
        || nTempBufferSize <= 0)
        return 0;

    int nRet = LZ4_decompress_safe(pSrcData, pTempBuffer, nSrcSize, static_cast<int>(nTempBufferSize));
    if (nRet > 0)
    {
        // 압축 풀기 성공!... 이지만
        if (nRet > nDstBufferSize)
            nRet = 0;
        else
            memcpy_s(pDstBuffer, static_cast<rsize_t>(nDstBufferSize), pTempBuffer, static_cast<rsize_t>(nRet));
    }
    else
    {
        nRet = 0;
    }

    return nRet;       // 압축 해제 후 원본 사이즈
}
