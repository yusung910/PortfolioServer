#pragma once
#include <vector>

class NetworkContextPO;
class NetworkHostPO;

class NetworkWorkerPO
{
private:
    volatile bool m_bIsTerminated = false;

    HANDLE m_hIOCP = INVALID_HANDLE_VALUE;
    std::vector<HANDLE> m_oHandleList;

    bool m_bIsStarted = false;

public:
    NetworkWorkerPO();
    virtual ~NetworkWorkerPO();

public:
    /*!
     *  IOCP 통신을 위한 Thread를 생성하는 함수
     *  CreateIoCompletionPort()로 Handle을 생성하고 m_hIOCP에 할당한다.
     *  서버의 CPU 프로세서 수 만큼 쓰레드를 생성하고 m_oHandleList에 push_back한다
     *  
     *      @return 성공 여부(성공 : true, 실패 :false)
     */
    bool CreateThread();

    /*!
     *  현재 진행중인 Thread를 종료하기 위한 함수
     *  PostQueuedCompletionStatus()를 호출해서 m_hIOCP 핸들에 완료 패킷을 게시
     *  m_bIsTerminated, m_bIsStarted에 false 값을 할당
     */
    void TerminateThread();

    /*!
     *  CreateThread() 내부에서 _beginthreadex()를 통해 Handle에 등록될 함수
     *  ProcessThread()와 TerminateThread()가 실행 된다
     *      @param [in,out] _arg 
     *
     *      @return 
     */
    static unsigned int WINAPI ExecuteThread(void* _arg);

    /*!
     *  인자값으로 전달받은 NetworkHostPO, NetworkContextPO를 IOCP
     *  PostQueuedCompletionStatus() 함수로 패킷을 전송한다
     *      @param [in,out] _host 
     *      @param [in,out] _ctxt 
     *
     *      @return 
     */
    bool PushThread(NetworkHostPO* _host, NetworkContextPO* _ctxt);

    /*!
     *  인자값으로 전달받은 NetworkHost를 CreateIoCompletionPort()함수를 이용해서
     *  스레드에 등록한다
     *
     *      @param [in,out] _host 
     *
     *      @return 
     */
    bool RegisterThread(NetworkHostPO* _host);

private:
    void ProcessThread();

    void ProcessAccept(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt);
    void ProcessConnect(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt);
    void ProcessReceive(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred);
    void ProcessEncrypt(NetworkHostPO& _host, NetworkContextPO& _ctxt);
    void ProcessSend(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred);
};

