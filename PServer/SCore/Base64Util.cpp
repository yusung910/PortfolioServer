#include "stdafx.hxx"
#include "Base64Util.h"

#include "StringUtil.h"

const char* Base64Util::BASE64_VALUES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


std::string Base64Util::Encode(const char* _srcBuffer, size_t _bufferSize)
{
    if (nullptr == _srcBuffer || _bufferSize == 0)
        return "";

    std::string lStrRet;
    lStrRet.reserve(_bufferSize * 2); // 

    size_t lRemainSize = _bufferSize;
    unsigned int lWorkBuffer = 0u;
    for (size_t i = 0; i < _bufferSize; i += BASE64_ENCODE_PAIR_SIZE)
    {
        lWorkBuffer = 0;
        size_t lCheckSumRemain = BASE64_ENCODE_PAIR_SIZE;

        for (int w = 0; w < BASE64_ENCODE_PAIR_SIZE; ++w)
        {
            if (lRemainSize > 0)
            {
                ((char*)(&lWorkBuffer))[3 - w] = _srcBuffer[i + w];
                lRemainSize--;
                lCheckSumRemain--;
            }
        }

        lStrRet += BASE64_VALUES[lWorkBuffer / BASE64_WORK_BIT_MASK];
        lWorkBuffer <<= BASE64_BIT_SHIFT_SIZE;
        lStrRet += BASE64_VALUES[lWorkBuffer / BASE64_WORK_BIT_MASK];

        if (lCheckSumRemain < 2)
        {
            lWorkBuffer <<= BASE64_BIT_SHIFT_SIZE;
            lStrRet += BASE64_VALUES[lWorkBuffer / BASE64_WORK_BIT_MASK];
        }
        else
        {
            lStrRet += '=';
        }

        if (lCheckSumRemain < 1)
        {
            lWorkBuffer <<= BASE64_BIT_SHIFT_SIZE;
            lStrRet += BASE64_VALUES[lWorkBuffer / BASE64_WORK_BIT_MASK];
        }
        else
        {
            lStrRet += '=';
        }
    }

    return lStrRet;
}

std::string Base64Util::Encode(const std::string& _srcBuffer)
{
    return Encode(_srcBuffer.c_str(), _srcBuffer.size());
}

size_t Base64Util::Decode(const char* _srcBuffer, const size_t& _srcSize, char* _destBuffer, const size_t& _destSize)
{
    if (nullptr == _srcBuffer
        || _srcSize == 0
        || nullptr == _destBuffer
        || _destSize == 0)
        return 0;

    std::string lWorkBuffer(_srcBuffer);

    //_srcBuffer가 4(BASE64_DECODE_PAIR_SIZE)의 배수가 아닐경우
    if (lWorkBuffer.size() % BASE64_DECODE_PAIR_SIZE != 0)
    {
        int lRemain = BASE64_DECODE_PAIR_SIZE - (int)(lWorkBuffer.size() % BASE64_DECODE_PAIR_SIZE);
        for (int i = 0; i < lRemain; i++)
        {
            lWorkBuffer.append("=");
        }
    }

    size_t lSrcSize = lWorkBuffer.size();

    char lTmp[BASE64_DECODE_PAIR_SIZE] = { 0, };
    size_t lWorkCnt = 0;

    for (size_t i = 0; i < lSrcSize; i += BASE64_DECODE_PAIR_SIZE)
    {
        memcpy_s(lTmp, BASE64_DECODE_PAIR_SIZE, lWorkBuffer.c_str() + i, BASE64_DECODE_PAIR_SIZE);


        if (i + BASE64_DECODE_PAIR_SIZE == lSrcSize)
        {
            _destBuffer[lWorkCnt++] = (char)((_GetBinaryValue(lTmp[0]) << 2) + (_GetBinaryValue(lTmp[1]) >> 4));

            if (lTmp[2] != '=')
                _destBuffer[lWorkCnt++] = (char)((_GetBinaryValue(lTmp[1]) << 4) + (_GetBinaryValue(lTmp[2]) >> 2));

            if (lTmp[3] != '=')
                _destBuffer[lWorkCnt++] = (char)((_GetBinaryValue(lTmp[2]) << 6) + (_GetBinaryValue(lTmp[3])));
        }
        else
        {
            _destBuffer[lWorkCnt++] = (char)((_GetBinaryValue(lTmp[0]) << 2) + (_GetBinaryValue(lTmp[1]) >> 4));
            _destBuffer[lWorkCnt++] = (char)((_GetBinaryValue(lTmp[1]) << 4) + (_GetBinaryValue(lTmp[2]) >> 2));
            _destBuffer[lWorkCnt++] = (char)((_GetBinaryValue(lTmp[2]) << 6) + (_GetBinaryValue(lTmp[3])));
        }
    }

    return lWorkCnt;
}

size_t Base64Util::Decode(const std::string& _srcBuffer, char* _destBuffer, const size_t& _destSize)
{
    return Decode(_srcBuffer.c_str(), _srcBuffer.size(), _destBuffer, _destSize);
}

const char Base64Util::_GetCode(int _nCode)
{
    if (_nCode < 0
        || _nCode >= BASE64_INDEX_SIZE)
        return '\0';


    return BASE64_VALUES[_nCode];
}

int Base64Util::_GetBinaryValue(const char& _ch)
{
    if (_ch >= 'A' && _ch <= 'Z')
        return (_ch - 'A');

    if (_ch >= 'a' && _ch <= 'z')
        return 26 + (_ch - 'a');

    if (_ch >= '0' && _ch <= '9')
        return 52 + (_ch - '0');

    if (_ch == '+' || _ch == '-')
        return 62;

    if (_ch == '/' || _ch == '_')
        return 63;

    return -1;
}
