/**
 *  @file NetworkContextPO.hxx
 *  @author YS
 *  @date 2024-10-14
 *  @project SCore
 *
 *  OVERLAPPED 구조체를 상속 받는다
 *  IOCP 통신에 사용되는 구조체
 * typedef struct _OVERLAPPED {
 * 
    ULONG_PTR Internal; I/O 요청에 대한 상태 코드

    ULONG_PTR InternalHigh; I/O 요청에 대해 전송된 바이트 수 

    union {
        struct {
            DWORD Offset; 사용자가 지정한 대로 I/O 요청을 시작할 파일 위치의 하위 부분
            DWORD OffsetHigh; 사용자가 지정한 대로 I/O 요청을 시작할 파일 위치의 상위 부분
        } DUMMYSTRUCTNAME;
        PVOID Pointer; 시스템 사용을 위해 예약됨; 를 0으로 초기화한 후에는 를 사용하지 마세요.
    } DUMMYUNIONNAME;  

    HANDLE  hEvent; 작업이 완료될 때 시스템에서 신호를 받는 상태로 설정되는 이벤트에 대한 핸들
} OVERLAPPED, *LPOVERLAPPED;
 *  https://learn.microsoft.com/ko-kr/windows/win32/api/minwinbase/ns-minwinbase-overlapped
 * 
 * 네트워크 통신에 필요한 정보를 정의하는 클래스
 */
#pragma once
#include "NetworkCommon.h"
#include <vector>

class NetworkContextPO : public OVERLAPPED
{
public:
    sockaddr_in* m_pLocalAddr = nullptr; //로컬 주소
    sockaddr_in* m_pRemoteAddr = nullptr; //원격 주소 

private:
    EContextType m_eType = EContextType::None;

    //volatile : https://m.blog.naver.com/classic2u/50003118713
    //volatile -> 휘발성 메모리에 변수를 생성
    //메모리에 직접 참조함으로써 변수의 값이 다양한 상황에 의해 변할 수 있음
    volatile long m_nReferenceCount;
    
    //HostID를 저장할 vector
    std::vector<int> m_oHostIDs;

    //서버 튜닝에 사용될 변수
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

    //Context정보 초기화
    void Reset();
    //Context의 Buffer 정보 초기화
    void ResetBuffer();

    //Context 참조 수 증가
    long IncreaseReferenceCount();
    //Context 참조 수 감소
    long DecreaseReferenceCount();

    //
    char* GetData();
    //
    size_t GetDataSize();
};

