#include "stdafx.hxx"
#include "NetworkContextPO.hxx"

NetworkContextPO::NetworkContextPO()
{
    Resize(NETWORK_BUFFER_SIZE_SERVER);
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
    if (m_nWriteSize <= 0 || m_nWriteSize <= m_nReadSize)
        return 0;

    if (nullptr == m_pBuffer)
        return 0;

    if (m_nWriteSize > m_nBufferSize)
        return 0;

    return m_nWriteSize - m_nReadSize;
}

char* NetworkContextPO::GetEmpty()
{
    if (nullptr == m_pBuffer)
        return nullptr;

    Align();

    if (m_nWriteSize > m_nBufferSize)
        return nullptr;

    return m_pBuffer + m_nWriteSize;
}

size_t NetworkContextPO::GetEmptySize()
{
    if (nullptr == m_pBuffer)
        return 0;

    Align();

    return m_nBufferSize - m_nWriteSize;
}

void NetworkContextPO::Ready(const EContextType& _type)
{
    Internal = 0;
    InternalHigh = 0;
    Offset = 0;
    OffsetHigh = 0;
    hEvent = 0;
    m_eType = _type;
}

void NetworkContextPO::Align()
{
    if (m_nReadSize <= 0) return;

    if (m_nReadSize < m_nWriteSize)
    {
        //https://modoocode.com/78#google_vignette
        //메모리 블록을 옮긴다
        //memmove(void* _Destination, void* _Source, size_t _Size) :
        //_Destination : 데이터가 복사될 곳을 가리키는 포인터로, 언제나 void* 형이다.
        //_Source : 복사할 데이터가 있는 위치를 가리키는 포인터로 언제나 void* 형이다.
        //_Size : 복사할 바이트 수.
        memmove(m_pBuffer, m_pBuffer + m_nReadSize, m_nWriteSize - m_nReadSize);
        m_nWriteSize -= m_nReadSize;
    }
    else
    {
        m_nWriteSize = 0;
    }

    m_nReadSize = 0;
}

void NetworkContextPO::Resize(const size_t& _size)
{
    if (m_nBufferSize >= _size)
        return;

    char* localpOldBuffer = m_pBuffer;
    m_pBuffer = new char[_size] {0, };

    if (nullptr != localpOldBuffer)
    {
        //memcpy_s(void *dest, size_t destSize, const void* src, size_t count)
        //void* dest : destination 줄임말, 복사 대상 버퍼
        //size_t destSize : 복사 대상의 버퍼 크기
        //const void* src : source 줄임말, 복사할 값을 가지고 있는 버퍼
        //size_t count : 복사할 바이트 수
        memcpy_s(m_pBuffer, _size, localpOldBuffer, m_nBufferSize);
        SafeDelete(localpOldBuffer);
    }

    m_nBufferSize = _size;
}

bool NetworkContextPO::Write(void* _src, const size_t& _srcSize)
{
    if (_src == nullptr || _srcSize <= 0)
        return false;

    if (_srcSize > m_nBufferSize)
        return false;

    memcpy_s(GetEmpty(), m_nBufferSize - m_nWriteSize, (char*)_src, _srcSize);

    m_nWriteSize += _srcSize;

    return true;
}

bool NetworkContextPO::Write(const size_t& _srcSize)
{
    if (_srcSize <= 0 || _srcSize > GetEmptySize())
        return false;

    m_nWriteSize += _srcSize;

    return true;
}

bool NetworkContextPO::Read(void* _dest, const size_t& _destSize)
{
    if (nullptr == _dest || _destSize <= 0)
        return false;

    char* localSrc = GetData();
    size_t localSrcSize = GetDataSize();

    if (localSrc == nullptr || localSrcSize < _destSize)
        return false;

    memcpy_s((char*)_dest, _destSize, localSrc, _destSize);
    m_nReadSize += _destSize;

    return true;
}

bool NetworkContextPO::Read(const size_t& _size)
{
    if (_size <= 0 || _size > GetDataSize())
        return false;

    m_nReadSize += _size;

    return true;
}

//

const EContextType& NetworkContextPO::GetContextType() const
{
    return m_eType;
}

std::vector<int>& NetworkContextPO::GetHostIDList()
{
    return m_oHostIDs;
}

void NetworkContextPO::SetHostIDList(const std::vector<int>& _hostIDs)
{
    m_oHostIDs.clear();
    m_oHostIDs.assign(_hostIDs.begin(), _hostIDs.end());
}

void NetworkContextPO::AddHostID(const int& _id)
{
    if (_id <= 0) return;

    m_oHostIDs.push_back(_id);
}
