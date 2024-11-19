#pragma once
#include "SCoreAPI.h"
#include <string>
#define DEFAULT_XOR_KEY "SERVER_XOR_STRING1234$#@!SDFEU@IJSDAKLAFHDSGLJHSdASJHe9eshfeawyefh"

class SCoreAPI XORUtil
{
private:
    char* m_pXORKey = nullptr;
    size_t m_nXORKeySize = 0;
public:
    XORUtil(const std::string& _strKey = DEFAULT_XOR_KEY);
    XORUtil(const char* _pKeyBuffer, size_t _bufferSize);
    ~XORUtil();

    bool Encrypt(const char* _srcBuffer, const size_t& _srcSize, char* _destBuffer, const size_t& _destSize);


private:
    void _MakeDefault();
};

