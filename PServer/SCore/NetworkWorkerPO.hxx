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
    /*!
     *  IOCP 통신을 통해 데이터를 받기 위한 스레드를 진행한다
     *  GetQueuedCompletionStatus() 함수로 패킷 데이터가 받아 올 때까지 대기한다.
     *  받아온 패킷 데이터의 클래스 NetworkHostPO, NetworkContextPO의
     *  EContextType에 해당하는 함수를 실행한다.
     *
     */
    void ProcessThread();

    /*!
     *  NetworkContextPO의 EContextType이 Accept일 때 실행하는 함수.
     *  ProcessThread()에서 GetQueuedCompletionStatus() 함수 실행 성공 했을 때
     *  NetworkHostPO, NetworkContextPO 객체와
     *  수신 성공 여부 값 (bool)이 _rslt 인자값으로 전달 된다.
     *  NetworkHostPO 객체에 저장된 Socket 옵션을 setsockopt() 함수로
     *  SO_UPDATE_ACCEPT_CONTEXT로 변경된 후
     *  NetworkContextPO에 있는 ip, port 등 주소 값을을 인자 값으로
     *  NetworkManager의 Join함수가 실행된다.
     *
     *  끝으로 NetworkContextPO의 Accept()를 실행한다
     *
     *      @param [in,out] _host
     *      @param [in,out] _ctxt
     *      @param [in]     _rslt
     */
    void ProcessAccept(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt);

    /*!
     *  NetworkContextPO의 EContextType이 Connect일 때 실행하는 함수.
     *  ProcessThread()에서 GetQueuedCompletionStatus() 함수 실행 성공 했을 때
     *  NetworkHostPO, NetworkContextPO 객체와
     *  수신 성공 여부 값 (bool)이 _rslt 인자값으로 전달 된다.
     *
     *  NetworkHostPO 객채의 EventConnect() 함수가 실행 된 후 Receive() 함수가 실행된다
     *
     *      @param [in,out] _host
     *      @param [in,out] _ctxt
     *      @param [in]     _rslt
     */
    void ProcessConnect(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt);


    /*!
     *  NetworkContextPO의 EContextType이 Receive일 때 실행하는 함수.
     *  ProcessThread()에서 GetQueuedCompletionStatus() 함수 실행 성공 했을 때
     *  NetworkHostPO, NetworkContextPO 객체와
     *  수신 성공 여부 값 (bool)이 _rslt 인자값 그리고
     *  IOCP 통신으로 전송된 데이터의 크기가 인자값으로 전달된다
     *  NetworkContextPO에 전송된 데이터의 크기를 추가로 기록하고
     *  NetworkHost에서 Decrypt함수로 복호화 처리한다
     *
     *      @param [in,out] _host
     *      @param [in,out] _ctxt
     *      @param [in]     _rslt
     *      @param [in]     _transferred
     */
    void ProcessReceive(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred);

    /*!
     *  NetworkContextPO의 EContextType이 Encrypt일 때 실행하는 함수.
     *  ProcessThread()에서 GetQueuedCompletionStatus() 함수 실행 성공 했을 때
     *  NetworkHostPO, NetworkContextPO 객체를 인자값으로 전달받는다
     *  NetworkContextPO 내부의 데이터를 NetworkHostPO에 해당하는 목적지로
     *  데이터를 보내며 데이터를 송신하기 위해 NetworkHost의 Encrypt함수로 암호화 하며
     *  암호화 후 보낼 때 NetworkHostPO의 Send()함수를 실행한다.
     *
     *      @param [in,out] _host
     *      @param [in,out] _ctxt
     */
    void ProcessEncrypt(NetworkHostPO& _host, NetworkContextPO& _ctxt);

    /*!
     *  NetworkContextPO의 EContextType이 Send일 때 실행하는 함수.
     *  ProcessThread()에서 GetQueuedCompletionStatus() 함수 실행 성공 했을 때
     *  NetworkHostPO, NetworkContextPO 객체를 인자값으로 전달받는다
     *
     *  NetworkHost 객체의 EndSendTask를 실행한다.
     *
     *      @param [in,out] _host
     *      @param [in,out] _ctxt
     *      @param [in]     _rslt
     *      @param [in]     _transferred
     */
    void ProcessSend(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred);
};

