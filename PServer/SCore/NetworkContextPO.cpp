#include "stdafx.hxx"
#include "NetworkContextPO.hxx"

NetworkContextPO::NetworkContextPO()
{
}

NetworkContextPO::~NetworkContextPO()
{
    SafeDeleteArray(m_pBuffer);
}

void NetworkContextPO::Reset()
{
    m_eType = EContextType::None;
    m_nReferenceCount = 0;
    m_oHostIDs.clear();

    ResetBuffer();

    m_pLocalAddr = nullptr;
    m_pRemoteAddr = nullptr;
}

void NetworkContextPO::ResetBuffer()
{
    m_nWriteSize = 0;
    m_nReadSize = 0;
}

long NetworkContextPO::IncreaseReferenceCount()
{
    //Interlocked~ 함수 : 스레드 간의 공유하고 있는 자원을 배타적으로 사용할 수 있도록 지원
    return InterlockedIncrement(&m_nReferenceCount);
}

long NetworkContextPO::DecreaseReferenceCount()
{
    return InterlockedDecrement(&m_nReferenceCount);
}

char* NetworkContextPO::GetData()
{
    //Context에 기록된 사이즈가 0보다 작거나 읽은 사이즈보다 작거나 같을경우 nullptr
    if (m_nWriteSize <= 0
        || m_nWriteSize <= m_nReadSize)
        return nullptr;

    if (nullptr == m_pBuffer)
        return nullptr;

    if (m_nWriteSize > m_nBufferSize)
        return nullptr;

    return m_pBuffer + m_nReadSize;
}

size_t NetworkContextPO::GetDataSize()
{
    return size_t();
}
