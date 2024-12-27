#pragma once
#include <string>
#include <Packet.h>

struct ConnectorTargetInfo
{
    int m_nHostID = 0;
    int m_nHostIP = 0;
    int m_nPort = 0;

    /*!
     *  문자열로 형식으로 전달받은 IP 주소를 2진수로 변경하여
     *  멤버 변수 m_nHostIP에 저장
     *
     *      @param [in] _ip
     */
    void SetIP(const std::string& _ip)
    {
        //IPv4 또는 IPv6 주소를 숫자 2진 형식으로 변경하는 함수
        InetPtonA(AF_INET, _ip.c_str(), &m_nHostIP);
    }

    /*!
     *  인자값으로 전달 받은 ip, port와
     *  멤버 변수에 저장된 ip, port가 동일한지 여부를 검사하는 함수
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