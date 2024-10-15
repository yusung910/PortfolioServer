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
 * 클라이언트와 서버 통신간에 필요한 데이터를 서버 컴퓨터 메모리에 기록하기 위한 클래스로 보임
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
    //데이터가 저장되는 변수
    char* m_pBuffer = nullptr;  

    //버퍼 사이즈
    size_t m_nBufferSize = 0;

    //기록된 buffer 크기
    size_t m_nWriteSize = 0;
    //읽은 buffer 크기
    size_t m_nReadSize = 0;

public:
    //Constructor
    NetworkContextPO();

    //Destructor
    virtual ~NetworkContextPO();

    //Context정보 초기화
    void Reset();
    //Context의 Buffer 사이즈 초기화
    void ResetBuffer();

    //Context 참조 수 증가
    long IncreaseReferenceCount();
    //Context 참조 수 감소
    long DecreaseReferenceCount();

    /*!
     *  NetworkContextPO::Read() 함수에 의해 읽혀진 데이터의 포인터를 반환
     *  
     *  m_pBuffer에서 m_nReadSize만큼 증가된 포인터 주소를 반환한다
     *      @return The data pointer.
     */
    char* GetData();

    /*!
     * NetworkContextPO::Read() 함수에 의해 읽혀진 데이터의 크기를 반환
     */
    size_t GetDataSize(); 

    /*!
     *  메모리가 비어있는 공간의 char* 포인터를 가져오기 위한 함수
     *  m_pBuffer의 nullptr 여부 확인 후 Align()로
     *  m_nWriteSize와 m_nReadSize의 사이즈를 조정한 후
     *  m_pBuffer에 기록할 포인터 주소를 반환한다
     *      @return The empty.
     */
    char* GetEmpty();

    /*!
     *  m_pBuffer에 비어있는 사이즈를 반환한다
     *
     *      @return The empty size.
     */
    size_t GetEmptySize();

    /*!
     *  IOCP 통신을 하기위한 OVERLAPPED 구조체의 데이터와 m_eType을 준비상태로 세팅
     *  인자값을 EContextType으로 전달받아 해당 상태로 세팅
     *      @param [in] EContextType& _type 
     */
    void Ready(const EContextType& _type);

    /*!
     *  NetworkContextPO::Read()와 NetworkContextPO::Write()함수에 의해
     *  기록(Write), 읽기(Read)된 데이터 만큼 m_pBuffer의 데이터를 메모리에서 이동시킨다
     */
    void Align();

    /*!
     *  기존에 m_pBuffer 저장된 데이터를 옮기고
     *  인자값으로 전달받은 _size만큼 m_pBuffer크기를 늘린다
     *
     *      @param [in] size_t& _size 
     */
    void Resize(const size_t& _size);

    /*!
     *  인자값으로 전달받은 _src 데이터를 기록한다
     *
     *      @param [in,out] _src     
     *      @param [in]     _srcSize 
     *
     *      @return 
     */
    bool Write(void* _src, const size_t& _srcSize);

    /*!
     *  _srcSize만큼 m_nWriteSize에 더한다
     *  기록할 데이터 크기만큼 m_nWriteSize 증가
     *      @param [in] _srcSize 
     *
     *      @return 
     */
    bool Write(const size_t& _srcSize);

    /*!
     *  m_pBuffer에 저장된 데이터를 _destSize 크기 만큼
     *  _dest에 반환한다
     *  
     *      @param [in,out] _dest     
     *      @param [in]     _destSize 
     *
     *      @return 
     */
    bool Read(void* _dest, const size_t& _destSize);

    /*!
     *  m_nReadSize에 _size만큼 더한다
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

