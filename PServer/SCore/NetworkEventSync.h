/**
 *  @file NetworkEventSync.h
 *  @author YS
 *  @date 2024-10-15
 *  @project SCore
 *
 *  네트워크 연결과 관련된 이벤트 싱크에 사용되는 클래스에서 상속 받아 사용하는 클래스
 *  각각 서버(billing, game 등)에서 NetworkConnect를 진행 할 때
 *  인자값으로 NetworkEventSync를 상속받는 클래스의 인스턴스를 전달할 때 사용한다.
 *  
 *  인터페이스 성격이 큼
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
     *  서버에서 NetworkManager로 Connect() 실행 할 때 발생할 이벤트 함수
     *
     *      @param [in] _hostID     
     *      @param [in] _ip         
     *      @param [in] _port       
     *      @param [in] _serverType 
     */
    virtual void OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType = 0) = 0;

    /*!
     *   
     *  서버에서 NetworkManager로 Close() 실행 할 때 발생할 이벤트 함수
     *
     *      @param [in] _hostID 
     */
    virtual void OnClose(const int& _hostID) = 0;
    
    
    /*!
     *  서버에서 NetworkManager에서 Receive 함수 실행 될 때 실행될 이벤트 함수
     */
    virtual void OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize) = 0;

    /*!
     *  
     *  서버 연결 시도 실패 했을 경우 
     *      @param [in] _hostID 
     */
    virtual void OnConnectedFailed(const int& _hostID)
    {
        OnClose(_hostID);
    }

    /*!
     *  Listen상태가 되었을 때
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
        //최소값 보정
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