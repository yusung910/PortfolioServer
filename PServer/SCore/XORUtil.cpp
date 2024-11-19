#include "stdafx.hxx"
#include "XORUtil.h"

XORUtil::XORUtil(const std::string& _strKey)
{
    if (true == _strKey.empty())
        _MakeDefault();
    else
    {
        m_nXORKeySize = _strKey.size();
        m_pXORKey = new char[m_nXORKeySize] {0, };
        memcpy_s(m_pXORKey, m_nXORKeySize, _strKey.c_str(), m_nXORKeySize);
    }
}

XORUtil::XORUtil(const char* _pKeyBuffer, size_t _bufferSize)
{
    if (nullptr == _pKeyBuffer ||
        _bufferSize == 0)
        _MakeDefault();
    else
    {
        m_nXORKeySize = _bufferSize;
        m_pXORKey = new char[m_nXORKeySize] {0, };
        memcpy_s(m_pXORKey, m_nXORKeySize, _pKeyBuffer, m_nXORKeySize);
    }
}

XORUtil::~XORUtil()
{
    SafeDelete(m_pXORKey);
}

bool XORUtil::Encrypt(const char* _srcBuffer, const size_t& _srcSize, char* _destBuffer, const size_t& _destSize)
{
    if (nullptr == m_pXORKey
        || 0 == m_nXORKeySize)
        return false;

    if (nullptr == _srcBuffer
        || 0 == _srcSize
        || nullptr == _destBuffer
        || 0 == _destSize
        || _srcBuffer > _destBuffer)
        return false;

    for (size_t i = 0; i < _srcSize; i++)
    { 
        _destBuffer[i] = _srcBuffer[i] ^ m_pXORKey[i % m_nXORKeySize];
    }
    
    return true;
}

void XORUtil::_MakeDefault()
{
    if (nullptr != m_pXORKey)
        return;

    std::string localTmp(DEFAULT_XOR_KEY);
    m_nXORKeySize = localTmp.size();
    m_pXORKey = new char[m_nXORKeySize] {0, };
    memcpy_s(m_pXORKey, m_nXORKeySize, localTmp.c_str(), m_nXORKeySize);
}
