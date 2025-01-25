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
        // ���� ����!... ������
        if (nRet > nDstBufferSize)
            nRet = 0;   // ��� ���� ����� �۾�
        else
            memcpy_s(pDstBuffer, static_cast<rsize_t>(nDstBufferSize), pTempBuffer, static_cast<rsize_t>(nRet));
    }
    else
    {
        nRet = 0; // ���� ����
    }

    return nRet;    // ����� ũ��
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
        // ���� Ǯ�� ����!... ������
        if (nRet > nDstBufferSize)
            nRet = 0;
        else
            memcpy_s(pDstBuffer, static_cast<rsize_t>(nDstBufferSize), pTempBuffer, static_cast<rsize_t>(nRet));
    }
    else
    {
        nRet = 0;
    }

    return nRet;       // ���� ���� �� ���� ������
}
