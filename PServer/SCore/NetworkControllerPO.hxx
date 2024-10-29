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
#include <Packet.h>

class NetworkHostPO;
class NetworkContextPO;

struct ConnectorTargetInfo
{
    int m_nHostID = 0;
    int m_nHostIP = 0;
    int m_nPort = 0;

    /*!
     *  ���ڿ��� �������� ���޹��� IP �ּҸ� 2������ �����Ͽ�
     *  ��� ���� m_nHostIP�� ����
     *
     *      @param [in] _ip 
     */
    void SetIP(const std::string& _ip)
    {
        //IPv4 �Ǵ� IPv6 �ּҸ� ���� 2�� �������� �����ϴ� �Լ�
        InetPtonA(AF_INET, _ip.c_str(), &m_nHostIP);
    }

    /*!
     *  ���ڰ����� ���� ���� ip, port��
     *  ��� ������ ����� ip, port�� �������� ���θ� �˻��ϴ� �Լ�
     *
     *      @param [in] _ip   
     *      @param [in] _port 
     *
     *      @return True if same. False if not.
     */
    bool IsSame(const std::string& _ip, int _port)
    {
        if (m_nPort != _port)
            return false;

        int localIP = 0;
        InetPtonA(AF_INET, _ip.c_str(), &localIP);

        return localIP == m_nHostIP;
    }

    bool IsSame(int _ip, int _port)
    {
        return (_ip != m_nHostIP || _port != m_nPort) ? false : true;
    }

};

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
     *  Adds the host.
     *
     *      @param [in,out] _host 
     *
     *      @return 
     */
    bool _AddHost(NetworkHostPO* _host);
};

