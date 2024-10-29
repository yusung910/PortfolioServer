/**
 *  @file NetworkHostPoolPO.hxx
 *  @author YS
 *  @date 2024-10-29
 *  @project SCore
 *
 *  NetworkHost�� pool�� �����ϱ� ���� Ŭ����
 */
#pragma once
#include <unordered_map>
#include <deque>
#include <mutex>

class NetworkHostPO;

class NetworkHostPoolPO
{
private:
    std::recursive_mutex m_xLock;
    int m_nLastHostID = 0;

    std::unordered_map<int, NetworkHostPO*> m_oActiveMap;
    std::deque<NetworkHostPO*> m_oFreeQueue;

public:
    NetworkHostPoolPO();
    virtual ~NetworkHostPoolPO();

    /*!
     *  HostID�� �����ؼ� m_oActiveMap�� HostID���� NetworkHostPO�� �Ҵ�
     *
     *      @return 
     */
    NetworkHostPO* Allocate();

    /*!
     *  ���ڰ����� ���� ���� NetworkHostPO�� m_oActiveMap���� ����� ������ ��
     *  m_oFreeQueue�� �߰��Ѵ�
     *
     *      @param [in,out] _host 
     */
    void Release(NetworkHostPO* _host);

    /*!
     *  ���ڰ����� ���޹��� HostID�� Ȱ��ȭ �� ��(m_oActiveMap)�� ���� ���θ� ��ȯ�Ѵ�
     *  true : ����, false : ����
     *      @param [in] _hostID 
     *
     *      @return 
     */
    bool Check(const int& _hostID);

    /*!
     *  ���ڰ����� ���޹��� HostID���� m_oActiveMap�� ����� NetworkHostPO�� ��ȯ�Ѵ�
     *
     *      @param [in] _hostID 
     *
     *      @return The host.
     */
    NetworkHostPO* GetHost(const int& _hostID);



};

