/**
 *  @file NetworkContextPO.hxx
 *  @author YS
 *  @date 2024-10-14
 *  @project SCore
 *
 *  OVERLAPPED ����ü�� ��� �޴´�
 *  IOCP ��ſ� ���Ǵ� ����ü
 * typedef struct _OVERLAPPED {
 * 
    ULONG_PTR Internal; I/O ��û�� ���� ���� �ڵ�

    ULONG_PTR InternalHigh; I/O ��û�� ���� ���۵� ����Ʈ �� 

    union {
        struct {
            DWORD Offset; ����ڰ� ������ ��� I/O ��û�� ������ ���� ��ġ�� ���� �κ�
            DWORD OffsetHigh; ����ڰ� ������ ��� I/O ��û�� ������ ���� ��ġ�� ���� �κ�
        } DUMMYSTRUCTNAME;
        PVOID Pointer; �ý��� ����� ���� �����; �� 0���� �ʱ�ȭ�� �Ŀ��� �� ������� ������.
    } DUMMYUNIONNAME;  

    HANDLE  hEvent; �۾��� �Ϸ�� �� �ý��ۿ��� ��ȣ�� �޴� ���·� �����Ǵ� �̺�Ʈ�� ���� �ڵ�
} OVERLAPPED, *LPOVERLAPPED;
 *  https://learn.microsoft.com/ko-kr/windows/win32/api/minwinbase/ns-minwinbase-overlapped
 * 
 * ��Ʈ��ũ ��ſ� �ʿ��� ������ �����ϴ� Ŭ����
 * Ŭ���̾�Ʈ�� ���� ��Ű��� �ʿ��� �����͸� ���� ��ǻ�� �޸𸮿� ����ϱ� ���� Ŭ������ ����
 */
#pragma once
#include "NetworkCommon.h"
#include <vector>

class NetworkContextPO : public OVERLAPPED
{
public:
    sockaddr_in* m_pLocalAddr = nullptr; //���� �ּ�
    sockaddr_in* m_pRemoteAddr = nullptr; //���� �ּ� 

private:
    EContextType m_eType = EContextType::None;

    //volatile : https://m.blog.naver.com/classic2u/50003118713
    //volatile -> �ֹ߼� �޸𸮿� ������ ����
    //�޸𸮿� ���� ���������ν� ������ ���� �پ��� ��Ȳ�� ���� ���� �� ����
    volatile long m_nReferenceCount;
    
    //HostID�� ������ vector
    std::vector<int> m_oHostIDs;

    //���� Ʃ�׿� ���� ����
    //�����Ͱ� ����Ǵ� ����
    char* m_pBuffer = nullptr;  

    //���� ������
    size_t m_nBufferSize = 0;

    //��ϵ� buffer ũ��
    size_t m_nWriteSize = 0;
    //���� buffer ũ��
    size_t m_nReadSize = 0;

public:
    //Constructor
    NetworkContextPO();

    //Destructor
    virtual ~NetworkContextPO();

    //Context���� �ʱ�ȭ
    void Reset();
    //Context�� Buffer ������ �ʱ�ȭ
    void ResetBuffer();

    //Context ���� �� ����
    long IncreaseReferenceCount();
    //Context ���� �� ����
    long DecreaseReferenceCount();

    /*!
     *  NetworkContextPO::Read() �Լ��� ���� ������ �������� �����͸� ��ȯ
     *  
     *  m_pBuffer���� m_nReadSize��ŭ ������ ������ �ּҸ� ��ȯ�Ѵ�
     *      @return The data pointer.
     */
    char* GetData();

    /*!
     * NetworkContextPO::Read() �Լ��� ���� ������ �������� ũ�⸦ ��ȯ
     */
    size_t GetDataSize(); 

    /*!
     *  �޸𸮰� ����ִ� ������ char* �����͸� �������� ���� �Լ�
     *  m_pBuffer�� nullptr ���� Ȯ�� �� Align()��
     *  m_nWriteSize�� m_nReadSize�� ����� ������ ��
     *  m_pBuffer�� ����� ������ �ּҸ� ��ȯ�Ѵ�
     *      @return The empty.
     */
    char* GetEmpty();

    /*!
     *  m_pBuffer�� ����ִ� ����� ��ȯ�Ѵ�
     *
     *      @return The empty size.
     */
    size_t GetEmptySize();

    /*!
     *  IOCP ����� �ϱ����� OVERLAPPED ����ü�� �����Ϳ� m_eType�� �غ���·� ����
     *  ���ڰ��� EContextType���� ���޹޾� �ش� ���·� ����
     *      @param [in] EContextType& _type 
     */
    void Ready(const EContextType& _type);

    /*!
     *  NetworkContextPO::Read()�� NetworkContextPO::Write()�Լ��� ����
     *  ���(Write), �б�(Read)�� ������ ��ŭ m_pBuffer�� �����͸� �޸𸮿��� �̵���Ų��
     */
    void Align();

    /*!
     *  ������ m_pBuffer ����� �����͸� �ű��
     *  ���ڰ����� ���޹��� _size��ŭ m_pBufferũ�⸦ �ø���
     *
     *      @param [in] size_t& _size 
     */
    void Resize(const size_t& _size);

    /*!
     *  ���ڰ����� ���޹��� _src �����͸� ����Ѵ�
     *
     *      @param [in,out] _src     
     *      @param [in]     _srcSize 
     *
     *      @return 
     */
    bool Write(void* _src, const size_t& _srcSize);

    /*!
     *  _srcSize��ŭ m_nWriteSize�� ���Ѵ�
     *  ����� ������ ũ�⸸ŭ m_nWriteSize ����
     *      @param [in] _srcSize 
     *
     *      @return 
     */
    bool Write(const size_t& _srcSize);

    /*!
     *  m_pBuffer�� ����� �����͸� _destSize ũ�� ��ŭ
     *  _dest�� ��ȯ�Ѵ�
     *  
     *      @param [in,out] _dest     
     *      @param [in]     _destSize 
     *
     *      @return 
     */
    bool Read(void* _dest, const size_t& _destSize);

    /*!
     *  m_nReadSize�� _size��ŭ ���Ѵ�
     *
     *      @param [in] _size 
     *
     *      @return 
     */
    bool Read(const size_t& _size);

public:
    const EContextType& GetContextType() const;
    std::vector<int>& GetHostIDList();
    void SetHostIDList(const std::vector<int>& _hostIDs);
    void AddHostID(const int& _id);
};

