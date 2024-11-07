/**
 *  @file NetworkControllerPO.hxx
 *  @author YS
 *  @date 2024-10-29
 *  @project SCore
 *  
 *  NetworkWorker�� NetworkHost�� ���� �����͸� �����ϸ�
 *  IOCP ����� �ϱ� ���� Ŭ����.
 *  EContextType�� ���� IOCP ����� �Ѵ�
 *  
 */
#pragma once

#include "Timer.h"
#include "BlockingQueue.h"
#include "SConnectorTargetInfo.h"


class NetworkHostPO;
class NetworkContextPO;

class NetworkControllerPO : public Timer
{
private:
    volatile bool m_bIsTerminated;
    HANDLE m_hNetworkControl = INVALID_HANDLE_VALUE;
    BlockingQueue<NetworkContextPO> m_oMsgQueue;

    std::mutex m_xRecvLock;
    std::mutex m_xHostIDLock;

    std::unordered_map<int, NetworkHostPO*> m_oHostList;
    std::vector<ConnectorTargetInfo> m_ConnectorHostList;

public:
    NetworkControllerPO();
    virtual ~NetworkControllerPO();

public:
    /*!
     *  NetworkController ������ �ڵ� ����
     *
     *      @return 
     */
    bool CreateThread();

    /*!
     *  NetworkController ������ �ߴ�
     */
    void TerminateThread();

    /*!
     *  CreateThread()�� ������ ������ �ڵ鿡�� ������ �Լ�
     *
     *      @param [in,out] _arg 
     *
     *      @return 
     */
    static unsigned int WINAPI ExcuteThread(void* _arg);

    /*!
     *  NetworkController���� ó���� NetworkContextPO�� �ִ� �Լ�
     *
     *      @param [in,out] _ctxt 
     *
     *      @return 
     */
    bool PushThread(NetworkContextPO* _ctxt);

    /*!
     *  m_ConnectorHostList�� ����� ������ ��
     *  ���ڰ����� ���޹��� ip�� port ���� ��ġ�ϴ� HostID���� ���T����
     *
     *      @param [in] _ip   
     *      @param [in] _port 
     *
     *      @return HostID
     */
    int GetConnectorHostID(const std::string& _ip, int _port);

    /*!
     *  ���ڰ����� ���޹��� HostID�� Packet�� ����
     *
     *      @param [in] _hostID 
     *      @param [in] _packet 
     *
     *      @return 
     */
    bool SendPacketToHost(const int& _hostID, Packet::SharedPtr _packet);


private:
    /*!
     *  ���ڰ����� ���޹��� NetworkHostPO�� Socket�� �����ϰ�
     *  ��� ���� m_oHostList �ʿ� NetworkHostPO�� ����ϸ�
     *  �� �� Socket ����� ���� NetworkWorker�� ����ϴ� �Լ�
     *
     *      @param [in,out] _host 
     *
     *      @return 
     */
    bool _AddHost(NetworkHostPO* _host);

    /*!
     *  ���ڰ����� ���޹��� hostID��
     *  Ŭ���� ������� m_oHostList�� ����Ǿ� �ִ� NetworkHostPO�� ã�� ��ȯ�Ѵ�
     *
     *      @param [in] _hostID
     *
     *      @return
     */
    NetworkHostPO* _FindHost(int _hostID);

    /*!
     *  m_oHostList�� ��ϵǾ� �ִ� NetworkHostPO�� IsAlive() ���� true�� ���
     *  Update�Լ��� ���������� �����ϰ�
     *  false�� ��� �ش� NetworkHostPO�� �����
     */
    void _UpdateHost();


    /*!
     *  ���ڰ����� ���޹��� NetworkHostPO�� ����Ǿ� �ִ� �����͵���
     *  ConnectorTargetInfo ����ü�� �����ϰ� �����ؼ�
     *  m_ConnectorHostList�� push_back�Ѵ�
     *
     *      @param [in,out] _host 
     */
    void _AddConnectorHost(NetworkHostPO* _host);

    /*!
     *  ���ڰ����� ���޹��� hostID ������ m_ConnectorHostList�� ����Ǿ� �ִ� ConnectorTargetInfo�� ã�Ƽ� �����
     *
     *      @param [in] _hostID 
     */
    void _RemoveConnectorHost(int _hostID);

private:
    /*!
     *  m_oMsgQueue�� ����� NetworkContextPO �����͸� ContextType�� �°� ó���ϴ� �Լ�
     *  ExcuteThread() �Լ� ���ο��� ���� �Ǹ�
     *  EContextType(Connect, Listen, Join, Close) ���� �ش��ϴ� �Լ��� ����ȴ�
     *  ���� �Լ��� m_oMsgQueue�� ����� NetworkContextPO Pointer�� ���� ������ �����Ͽ� �����Ѵ�
     */
    void ProcessThread();

    /*!
     *  ���� ������ ���޹��� NetworkContextPO�� ���� ����ϱ� ���� ConnectEx�Լ���
     *  ������ �����ϴ� �Լ�
     *  
     *      @param [in,out] _ctxt 
     */
    void ProcessConnect(NetworkContextPO& _ctxt);

    /*!
     *  ���� ������ ���޹��� NetworkContextPO��
     *  NetworkHostPO�� Listen ���·� �����ϴ� �Լ�
     *
     *      @param [in,out] _ctxt 
     */
    void ProcessListen(NetworkContextPO& _ctxt);

    /*!
     *  ���� ������ ���޹��� NetworkContextPO�� NetworkHostPO���� WSARecv() �Լ��� ����
     *  Socket �����͸� ���� ó���Ѵ�
     *  
     *      @param [in,out] _ctxt 
     */
    void ProcessJoin(NetworkContextPO& _ctxt);

    /*!
     *  ���� ������ ���޹��� NetworkContextPO�� NetworkHostPO�� Socket Close ó���Ѵ�
     *
     *      @param [in,out] _ctxt 
     */
    void ProcessClose(NetworkContextPO& _ctxt);
};

