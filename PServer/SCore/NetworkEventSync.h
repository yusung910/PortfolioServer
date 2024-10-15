/**
 *  @file NetworkEventSync.h
 *  @author YS
 *  @date 2024-10-15
 *  @project SCore
 *
 *  ��Ʈ��ũ ����� ���õ� �̺�Ʈ ��ũ�� ���Ǵ� Ŭ�������� ��� �޾� ����ϴ� Ŭ����
 *  ���� ����(billing, game ��)���� NetworkConnect�� ���� �� ��
 *  ���ڰ����� NetworkEventSync�� ��ӹ޴� Ŭ������ �ν��Ͻ��� ������ �� ����Ѵ�.
 *  
 *  �������̽� ������ ŭ
 */
#pragma once
#include "SCoreAPI.h"
#include <string>
#include "NetworkCommon.h"

class SCoreAPI NetworkEventSync
{
private:
    int m_nTimeoutMS = DEFAULT_HOST_TIMEOUT_MS;

public:
    NetworkEventSync() = default;
    virtual ~NetworkEventSync() = default;

    /*!
     *  On connect.
     *
     *      @param [in] _hostID     
     *      @param [in] _ip         
     *      @param [in] _port       
     *      @param [in] _serverType 
     */
    virtual void OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType = 0) = 0;

    /*!
     *  On close.
     *
     *      @param [in] _hostID 
     */
    virtual void OnClose(const int& _hostID) = 0;
    
    
    /*!
     *  
     */
    virtual void OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize) = 0;

    /*!
     *  On connected failed.
     *  ���� ���� �õ� ���� ���� ��� 
     *      @param [in] _hostID 
     */
    virtual void OnConnectedFailed(const int& _hostID)
    {
        OnClose(_hostID);
    }

    /*!
     *  
     */
    virtual void OnListen() {};

    /*!
     *  
     */
    virtual void OnListenFailed() {};

    
    /*!
     *  Sets the network event sync's timeout ms.
     *
     *      @param [in] _ms 
     *
     *      @return 
     */
    void SetTimeoutMS(const int& _ms = DEFAULT_HOST_TIMEOUT_MS) noexcept
    {
        //�ּҰ� ����
        if (MIN_HOST_TIMEOUT_MS < _ms)
            m_nTimeoutMS = _ms;
    }

    /*!
     *  Returns the network event sync's timeout m s.
     *
     *      @return The timeout m s.
     */
    const int& GetTimeoutMS() const noexcept
    {
        return m_nTimeoutMS;
    }

    

};