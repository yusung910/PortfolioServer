/**
 *  @file NetworkControllerPO.hxx
 *  @author YS
 *  @date 2024-10-29
 *  @project SCore
 *
 *  IOCP ����� ���� WSASocket ���� ��ɵ��� �����ϴ� Ŭ����
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
     *  ������ ����
     *
     *      @return 
     */
    bool CreateThread();
    void TerminateThread();

    static unsigned int WINAPI ExcuteThread(void* _arg);


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

    void ProcessThread();
};

