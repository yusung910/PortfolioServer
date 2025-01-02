#pragma once
#include <NetworkEventSync.h>
class LoginServerNES : public NetworkEventSync
{
public:
    LoginServerNES() = default;
    virtual ~LoginServerNES() = default;

    /*!
     *  서버에서 NetworkManager로 Connect() 실행 할 때 발생할 이벤트 함수
     *
     *      @param [in] _hostID
     *      @param [in] _ip
     *      @param [in] _port
     *      @param [in] _serverType
     */
    virtual void OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType = 0);

    /*!
     *
     *  서버에서 NetworkManager로 Close() 실행 할 때 발생할 이벤트 함수
     *
     *      @param [in] _hostID
     */
    virtual void OnClose(const int& _hostID);


    /*!
     *  서버에서 NetworkManager에서 Receive 함수 실행 될 때 실행될 이벤트 함수
     */
    virtual void OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize);
};

