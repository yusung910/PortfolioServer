#pragma once
#include <NetworkEventSync.h>
class LoginServerNES : public NetworkEventSync
{
public:
    LoginServerNES() = default;
    virtual ~LoginServerNES() = default;

    /*!
     *  �������� NetworkManager�� Connect() ���� �� �� �߻��� �̺�Ʈ �Լ�
     *
     *      @param [in] _hostID
     *      @param [in] _ip
     *      @param [in] _port
     *      @param [in] _serverType
     */
    virtual void OnConnect(const int& _hostID, const std::string& _ip, const int& _port, const int& _serverType = 0);

    /*!
     *
     *  �������� NetworkManager�� Close() ���� �� �� �߻��� �̺�Ʈ �Լ�
     *
     *      @param [in] _hostID
     */
    virtual void OnClose(const int& _hostID);


    /*!
     *  �������� NetworkManager���� Receive �Լ� ���� �� �� ����� �̺�Ʈ �Լ�
     */
    virtual void OnReceive(const int& _hostID, const int& _msgID, char* _msg, const int& _msgSize);
};

