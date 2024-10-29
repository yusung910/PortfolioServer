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
     *  IOCP ����� ���� Thread�� �����ϴ� �Լ�
     *  CreateIoCompletionPort()�� Handle�� �����ϰ� m_hIOCP�� �Ҵ��Ѵ�.
     *  ������ CPU ���μ��� �� ��ŭ �����带 �����ϰ� m_oHandleList�� push_back�Ѵ�
     *  
     *      @return ���� ����(���� : true, ���� :false)
     */
    bool CreateThread();

    /*!
     *  ���� �������� Thread�� �����ϱ� ���� �Լ�
     *  PostQueuedCompletionStatus()�� ȣ���ؼ� m_hIOCP �ڵ鿡 �Ϸ� ��Ŷ�� �Խ�
     *  m_bIsTerminated, m_bIsStarted�� false ���� �Ҵ�
     */
    void TerminateThread();

    /*!
     *  CreateThread() ���ο��� _beginthreadex()�� ���� Handle�� ��ϵ� �Լ�
     *  ProcessThread()�� TerminateThread()�� ���� �ȴ�
     *      @param [in,out] _arg 
     *
     *      @return 
     */
    static unsigned int WINAPI ExecuteThread(void* _arg);

    /*!
     *  ���ڰ����� ���޹��� NetworkHostPO, NetworkContextPO�� IOCP
     *  PostQueuedCompletionStatus() �Լ��� ��Ŷ�� �����Ѵ�
     *      @param [in,out] _host 
     *      @param [in,out] _ctxt 
     *
     *      @return 
     */
    bool PushThread(NetworkHostPO* _host, NetworkContextPO* _ctxt);

    /*!
     *  ���ڰ����� ���޹��� NetworkHost�� CreateIoCompletionPort()�Լ��� �̿��ؼ�
     *  �����忡 ����Ѵ�
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

