#pragma once
#include "SCoreAPI.h"


class SCoreAPI LZ4Compressor
{
public:
    LZ4Compressor() = delete;
    LZ4Compressor(const LZ4Compressor&) = delete;
    ~LZ4Compressor() = delete;
    
    /*!
     * 데이터를 압축한다. 입력값이 잘못되거나, 버퍼가 작거나 할 경우 실패한다.
     * 압축을 했지만 압축 이후 크기가 더 큰경우도 실패처리.
     *
     * 원본 데이터를 압축한 뒤, 원본 데이터의 버퍼에 압축한 데이터를 덮어쓴다.
     *
     *      @param [in,out] pSrcDstData_io      : 원본 데이터 및 출력 데이터
     *      @param [in]     nSrcSize            : 압축 데이터 크기
     *      @param [in]     nSrcBufferMaxSize   : 버퍼 데이터 크기
     *      @param [in,out] pTempBuffer         : 압축에 사용 될 임시 버퍼
     *      @param [in]     nTempBufferSize     : 임시 버퍼 사이즈
     *
     *      @return
     */
    static int Compress(char* pSrcDstData_io, const int& nSrcSize, const int& nSrcBufferMaxSize, char* pTempBuffer, const int& nTempBufferSize);

    /*!
     *  압축된 데이터를 해제한다. 입력값이 잘못되거나, 버퍼가 작거나 할 경우 실패한다.
     *  압축을 해제한 뒤, 압축된 입력 버퍼에 압축 해제한 데이터를 덮어쓴다.
     *
     *      @param [in,out] pSrcDstData_io      : 원본 데이터 및 출력 데이터
     *      @param [in]     nSrcSize            : 원본 데이터 사이즈
     *      @param [in]     nSrcBufferMaxSize   : 버퍼 데이터 크기
     *      @param [in,out] pTempBuffer         : 임시 버퍼
     *      @param [in]     nTempBufferSize     : 임시 버퍼 사이즈
     *
     *      @return
     */
    static int Decompress(char* pSrcDstData_io, const int& nSrcSize, const int& nSrcBufferMaxSize, char* pTempBuffer, const int& nTempBufferSize);

    /*!
     * 데이터를 압축한다. 입력값이 잘못되거나, 버퍼가 작거나 할 경우 실패한다.
     * 압축을 했지만 압축 이후 크기가 더 큰경우도 실패처리.
     *
     * 원본 데이터를 압축한 뒤, 원본 데이터의 버퍼에 압축한 데이터를 덮어쓴다.
     *
     *      @param [in]     pSrcData        : 원본 데이터
     *      @param [in]     nSrcSize        : 원본 데이터 크기
     *      @param [in,out] pDstBuffer      : 압축 결과 데이터
     *      @param [in]     nDstBufferSize  : 압축 결과 데이터 크기
     *      @param [in,out] pTempBuffer     : 임시 버퍼
     *      @param [in]     nTempBufferSize : 임시 버퍼 크기
     *
     *      @return
     */
    static int Compress(const char* pSrcData, const int& nSrcSize, char* pDstBuffer, const int& nDstBufferSize, char* pTempBuffer, const int& nTempBufferSize);

    /*!
     *  압축된 데이터를 해제한다. 입력값이 잘못되거나, 버퍼가 작거나 할 경우 실패한다.
     *  압축을 해제한 뒤, 압축된 입력 버퍼에 압축 해제한 데이터를 덮어쓴다.
     *
     *      @param [in]     pSrcData        : 원본 데이터
     *      @param [in]     nSrcSize        : 원본 데이터 크기
     *      @param [in,out] pDstBuffer      : 압축 결과 데이터
     *      @param [in]     nDstBufferSize  : 압축 결과 데이터 크기
     *      @param [in,out] pTempBuffer     : 임시 버퍼
     *      @param [in]     nTempBufferSize : 임시 버퍼 크기
     *
     *      @return
     */
    static int Decompress(const char* pSrcData, const int& nSrcSize, char* pDstBuffer, const int& nDstBufferSize, char* pTempBuffer, const int& nTempBufferSize);
};

