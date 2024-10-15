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
    //Interlocked~ �Լ� : ������ ���� �����ϰ� �ִ� �ڿ��� ��Ÿ������ ����� �� �ֵ��� ����
    return InterlockedIncrement(&m_nReferenceCount);
}

long NetworkContextPO::DecreaseReferenceCount()
{
    return InterlockedDecrement(&m_nReferenceCount);
}

char* NetworkContextPO::GetData()
{
    //Context�� ��ϵ� ����� 0���� �۰ų� ���� ������� �۰ų� ������� nullptr
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
