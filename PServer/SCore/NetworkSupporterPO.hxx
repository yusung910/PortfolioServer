/**
 *  @file NetworkSupporterPO.hxx
 *  @author YS
 *  @date 2024-10-16
 *  @project SCore
 *
 *  Winsock(소켓) 통신에 필요한 서폿 역할 함수를 제공하는 클래스
 */
#pragma once
#include <string>
#include <vector>
#include <winsock.h>

class NetworkSupporterPO
{
public:
    NetworkSupporterPO();
    virtual ~NetworkSupporterPO();

public:
    /*!
     *  서버의 로컬 아이피를 반환 하는 함수
     *
     *      @param [in,out] _addr
     */
    static void GetLocalIPAddress(std::vector<std::string>& _addr);
    /*!
     *  ip와 port를 인자값으로 전달받아서
     *
     *      @param [in] _ip
     *      @param [in] _port
     *
     *      @return The address info.
     */
    static SOCKADDR_IN GetAddressInfo(std::string _ip, int _port);
};