#pragma once
#include <NetworkEventSync.h>

class GServerCheckNES : public NetworkEventSync
{
public:
    GServerCheckNES() = default;
    virtual ~GServerCheckNES() = default;

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

