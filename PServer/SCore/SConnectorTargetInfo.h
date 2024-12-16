#pragma once
#include <string>
#include <Packet.h>

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

        int lIP = 0;
        InetPtonA(AF_INET, _ip.c_str(), &lIP);

        return lIP == m_nHostIP;
    }

    bool IsSame(int _ip, int _port)
    {
        return (_ip != m_nHostIP || _port != m_nPort) ? false : true;
    }

};