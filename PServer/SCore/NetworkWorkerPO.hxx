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
    /*!
     *  IOCP ����� ���� �����͸� �ޱ� ���� �����带 �����Ѵ�
     *  GetQueuedCompletionStatus() �Լ��� ��Ŷ �����Ͱ� �޾� �� ������ ����Ѵ�.
     *  �޾ƿ� ��Ŷ �������� Ŭ���� NetworkHostPO, NetworkContextPO��
     *  EContextType�� �ش��ϴ� �Լ��� �����Ѵ�.
     *  
     */
    void ProcessThread();

    /*!
     *  NetworkContextPO�� EContextType�� Accept�� �� �����ϴ� �Լ�.
     *  ProcessThread()���� GetQueuedCompletionStatus() �Լ� ���� ���� ���� ��
     *  NetworkHostPO, NetworkContextPO ��ü��
     *  ���� ���� ���� �� (bool)�� _rslt ���ڰ����� ���� �ȴ�.
     *  NetworkHostPO ��ü�� ����� Socket �ɼ��� setsockopt() �Լ���
     *  SO_UPDATE_ACCEPT_CONTEXT�� ����� ��
     *  NetworkContextPO�� �ִ� ip, port �� �ּ� ������ ���� ������
     *  NetworkManager�� Join�Լ��� ����ȴ�.
     *
     *  ������ NetworkContextPO�� Accept()�� �����Ѵ�
     *  
     *      @param [in,out] _host 
     *      @param [in,out] _ctxt 
     *      @param [in]     _rslt 
     */
    void ProcessAccept(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt);

    /*!
     *  NetworkContextPO�� EContextType�� Connect�� �� �����ϴ� �Լ�.
     *  ProcessThread()���� GetQueuedCompletionStatus() �Լ� ���� ���� ���� ��
     *  NetworkHostPO, NetworkContextPO ��ü��
     *  ���� ���� ���� �� (bool)�� _rslt ���ڰ����� ���� �ȴ�.
     *  
     *  NetworkHostPO ��ä�� EventConnect() �Լ��� ���� �� �� Receive() �Լ��� ����ȴ�
     *  
     *      @param [in,out] _host 
     *      @param [in,out] _ctxt 
     *      @param [in]     _rslt 
     */
    void ProcessConnect(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt);


    /*!
     *  NetworkContextPO�� EContextType�� Receive�� �� �����ϴ� �Լ�.
     *  ProcessThread()���� GetQueuedCompletionStatus() �Լ� ���� ���� ���� ��
     *  NetworkHostPO, NetworkContextPO ��ü��
     *  ���� ���� ���� �� (bool)�� _rslt ���ڰ� �׸���
     *  IOCP ������� ���۵� �������� ũ�Ⱑ ���ڰ����� ���޵ȴ�
     *  NetworkContextPO�� ���۵� �������� ũ�⸦ �߰��� ����ϰ�
     *  NetworkHost���� Decrypt�Լ��� ��ȣȭ ó���Ѵ�
     *  
     *      @param [in,out] _host
     *      @param [in,out] _ctxt
     *      @param [in]     _rslt
     *      @param [in]     _transferred
     */
    void ProcessReceive(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred);

    /*!
     *  NetworkContextPO�� EContextType�� Encrypt�� �� �����ϴ� �Լ�.
     *  ProcessThread()���� GetQueuedCompletionStatus() �Լ� ���� ���� ���� ��
     *  NetworkHostPO, NetworkContextPO ��ü�� ���ڰ����� ���޹޴´�
     *  NetworkContextPO ������ �����͸� NetworkHostPO�� �ش��ϴ� ��������
     *  �����͸� ������ �����͸� �۽��ϱ� ���� NetworkHost�� Encrypt�Լ��� ��ȣȭ �ϸ�
     *  ��ȣȭ �� ���� �� NetworkHostPO�� Send()�Լ��� �����Ѵ�.
     *  
     *      @param [in,out] _host 
     *      @param [in,out] _ctxt 
     */
    void ProcessEncrypt(NetworkHostPO& _host, NetworkContextPO& _ctxt);

    /*!
     *  NetworkContextPO�� EContextType�� Send�� �� �����ϴ� �Լ�.
     *  ProcessThread()���� GetQueuedCompletionStatus() �Լ� ���� ���� ���� ��
     *  NetworkHostPO, NetworkContextPO ��ü�� ���ڰ����� ���޹޴´�
     *
     *  NetworkHost ��ü�� EndSendTask�� �����Ѵ�.
     *  
     *      @param [in,out] _host
     *      @param [in,out] _ctxt
     *      @param [in]     _rslt
     *      @param [in]     _transferred
     */
    void ProcessSend(NetworkHostPO& _host, NetworkContextPO& _ctxt, bool _rslt, int _transferred);
};

