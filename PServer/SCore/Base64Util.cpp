#include "stdafx.hxx"
#include "Base64Util.h"

#include "StringUtil.h"

const char* Base64Util::BASE64_VALUES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


std::string Base64Util::Encode(const char* _srcBuffer, size_t _bufferSize)
{
    if (nullptr == _srcBuffer || _bufferSize == 0)
        return "";

    std::string localStrRet;
    localStrRet.reserve(_bufferSize * 2); // 

    size_t localRemainSize = _bufferSize;
    unsigned int localWorkBuffer = 0u;
    for (size_t i = 0; i < _bufferSize; i += BASE64_ENCODE_PAIR_SIZE)
    {
        localWorkBuffer = 0;
        size_t localCheckSumRemain = BASE64_ENCODE_PAIR_SIZE;

        for (int w = 0; w < BASE64_ENCODE_PAIR_SIZE; ++w)
        {
            if (localRemainSize > 0)
            {
                ((char*)(&localWorkBuffer))[3 - w] = _srcBuffer[i + w];
                localRemainSize--;
                localCheckSumRemain--;
            }
        }

        localStrRet += BASE64_VALUES[localWorkBuffer / BASE64_WORK_BIT_MASK];
        localWorkBuffer <<= BASE64_BIT_SHIFT_SIZE;
        localStrRet += BASE64_VALUES[localWorkBuffer / BASE64_WORK_BIT_MASK];

        if (localCheckSumRemain < 2)
        {
            localWorkBuffer <<= BASE64_BIT_SHIFT_SIZE;
            localStrRet += BASE64_VALUES[localWorkBuffer / BASE64_WORK_BIT_MASK];
        }
        else
        {
            localStrRet += '=';
        }

        if (localCheckSumRemain < 1)
        {
            localWorkBuffer <<= BASE64_BIT_SHIFT_SIZE;
            localStrRet += BASE64_VALUES[localWorkBuffer / BASE64_WORK_BIT_MASK];
        }
        else
        {
            localStrRet += '=';
        }
    }

    return localStrRet;
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

    std::string localWorkBuffer(_srcBuffer);

    //_srcBuffer가 4(BASE64_DECODE_PAIR_SIZE)의 배수가 아닐경우
    if (localWorkBuffer.size() % BASE64_DECODE_PAIR_SIZE != 0)
    {
        int localRemain = BASE64_DECODE_PAIR_SIZE - (int)(localWorkBuffer.size() % BASE64_DECODE_PAIR_SIZE);
        for (int i = 0; i < localRemain; i++)
        {
            localWorkBuffer.append("=");
        }
    }

    size_t localSrcSize = localWorkBuffer.size();

    char localTmp[BASE64_DECODE_PAIR_SIZE] = { 0, };
    size_t localWorkCnt = 0;

    for (size_t i = 0; i < localSrcSize; i+= BASE64_DECODE_PAIR_SIZE)
    {
        memcpy_s(localTmp, BASE64_DECODE_PAIR_SIZE, localWorkBuffer.c_str() + i, BASE64_DECODE_PAIR_SIZE);


        if (i + BASE64_DECODE_PAIR_SIZE == localSrcSize)
        {
            _destBuffer[localWorkCnt++] = (char)((_GetBinaryValue(localTmp[0]) << 2) + (_GetBinaryValue(localTmp[1]) >> 4));

            if(localTmp[2] != '=')
                _destBuffer[localWorkCnt++] = (char)((_GetBinaryValue(localTmp[1]) << 4) + (_GetBinaryValue(localTmp[2]) >> 2));

            if (localTmp[3] != '=')
                _destBuffer[localWorkCnt++] = (char)((_GetBinaryValue(localTmp[2]) << 6) + (_GetBinaryValue(localTmp[3])));
        }
        else
        {
            _destBuffer[localWorkCnt++] = (char)((_GetBinaryValue(localTmp[0]) << 2) + (_GetBinaryValue(localTmp[1]) >> 4));
            _destBuffer[localWorkCnt++] = (char)((_GetBinaryValue(localTmp[1]) << 4) + (_GetBinaryValue(localTmp[2]) >> 2));
            _destBuffer[localWorkCnt++] = (char)((_GetBinaryValue(localTmp[2]) << 6) + (_GetBinaryValue(localTmp[3])));
        }
    }

    return localWorkCnt;
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
