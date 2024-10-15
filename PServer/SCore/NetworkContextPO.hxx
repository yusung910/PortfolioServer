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
    char* m_pBuffer = nullptr;  

    //
    size_t m_nBufferSize = 0;

    //
    size_t m_nWriteSize = 0;
    //
    size_t m_nReadSize = 0;

public:
    //Constructor
    NetworkContextPO();

    //Destructor
    virtual ~NetworkContextPO();

    //Context���� �ʱ�ȭ
    void Reset();
    //Context�� Buffer ���� �ʱ�ȭ
    void ResetBuffer();

    //Context ���� �� ����
    long IncreaseReferenceCount();
    //Context ���� �� ����
    long DecreaseReferenceCount();

    //
    char* GetData();
    //
    size_t GetDataSize();
};

