#pragma once
#include "SCoreAPI.h"
#include <string>

// 구현 참고 : https://tools.ietf.org/html/rfc1421
// 최적화 필요

constexpr size_t BASE64_DECODE_PAIR_SIZE = 4;
constexpr size_t BASE64_INDEX_SIZE = 64;
constexpr size_t BASE64_ENCODE_PAIR_SIZE = 3;
constexpr unsigned int BASE64_WORK_BIT_MASK = 0x04000000;
constexpr size_t BASE64_BIT_SHIFT_SIZE = 6;


class SCoreAPI Base64Util
{
private:
    static const char* BASE64_VALUES;

public:
    static std::string Encode(const char* _srcBuffer, size_t _bufferSize);
    static std::string Encode(const std::string& _srcBuffer);

    static size_t Decode(const char* _srcBuffer, const size_t& _srcSize, char* _destBuffer, const size_t& _destSize);
    static size_t Decode(const std::string& _srcBuffer, char* _destBuffer, const size_t& _destSize);

private:
    static const char _GetCode(int _nCode);
    static int _GetBinaryValue(const char& _ch);
};

