#pragma once
#include "SCoreAPI.h"


class SCoreAPI LZ4Compressor
{
public:
    LZ4Compressor() = delete;
    LZ4Compressor(const LZ4Compressor&) = delete;
    ~LZ4Compressor() = delete;
    
    /*!
     * �����͸� �����Ѵ�. �Է°��� �߸��ǰų�, ���۰� �۰ų� �� ��� �����Ѵ�.
     * ������ ������ ���� ���� ũ�Ⱑ �� ū��쵵 ����ó��.
     *
     * ���� �����͸� ������ ��, ���� �������� ���ۿ� ������ �����͸� �����.
     *
     *      @param [in,out] pSrcDstData_io      : ���� ������ �� ��� ������
     *      @param [in]     nSrcSize            : ���� ������ ũ��
     *      @param [in]     nSrcBufferMaxSize   : ���� ������ ũ��
     *      @param [in,out] pTempBuffer         : ���࿡ ��� �� �ӽ� ����
     *      @param [in]     nTempBufferSize     : �ӽ� ���� ������
     *
     *      @return
     */
    static int Compress(char* pSrcDstData_io, const int& nSrcSize, const int& nSrcBufferMaxSize, char* pTempBuffer, const int& nTempBufferSize);

    /*!
     *  ����� �����͸� �����Ѵ�. �Է°��� �߸��ǰų�, ���۰� �۰ų� �� ��� �����Ѵ�.
     *  ������ ������ ��, ����� �Է� ���ۿ� ���� ������ �����͸� �����.
     *
     *      @param [in,out] pSrcDstData_io      : ���� ������ �� ��� ������
     *      @param [in]     nSrcSize            : ���� ������ ������
     *      @param [in]     nSrcBufferMaxSize   : ���� ������ ũ��
     *      @param [in,out] pTempBuffer         : �ӽ� ����
     *      @param [in]     nTempBufferSize     : �ӽ� ���� ������
     *
     *      @return
     */
    static int Decompress(char* pSrcDstData_io, const int& nSrcSize, const int& nSrcBufferMaxSize, char* pTempBuffer, const int& nTempBufferSize);

    /*!
     * �����͸� �����Ѵ�. �Է°��� �߸��ǰų�, ���۰� �۰ų� �� ��� �����Ѵ�.
     * ������ ������ ���� ���� ũ�Ⱑ �� ū��쵵 ����ó��.
     *
     * ���� �����͸� ������ ��, ���� �������� ���ۿ� ������ �����͸� �����.
     *
     *      @param [in]     pSrcData        : ���� ������
     *      @param [in]     nSrcSize        : ���� ������ ũ��
     *      @param [in,out] pDstBuffer      : ���� ��� ������
     *      @param [in]     nDstBufferSize  : ���� ��� ������ ũ��
     *      @param [in,out] pTempBuffer     : �ӽ� ����
     *      @param [in]     nTempBufferSize : �ӽ� ���� ũ��
     *
     *      @return
     */
    static int Compress(const char* pSrcData, const int& nSrcSize, char* pDstBuffer, const int& nDstBufferSize, char* pTempBuffer, const int& nTempBufferSize);

    /*!
     *  ����� �����͸� �����Ѵ�. �Է°��� �߸��ǰų�, ���۰� �۰ų� �� ��� �����Ѵ�.
     *  ������ ������ ��, ����� �Է� ���ۿ� ���� ������ �����͸� �����.
     *
     *      @param [in]     pSrcData        : ���� ������
     *      @param [in]     nSrcSize        : ���� ������ ũ��
     *      @param [in,out] pDstBuffer      : ���� ��� ������
     *      @param [in]     nDstBufferSize  : ���� ��� ������ ũ��
     *      @param [in,out] pTempBuffer     : �ӽ� ����
     *      @param [in]     nTempBufferSize : �ӽ� ���� ũ��
     *
     *      @return
     */
    static int Decompress(const char* pSrcData, const int& nSrcSize, char* pDstBuffer, const int& nDstBufferSize, char* pTempBuffer, const int& nTempBufferSize);
};

