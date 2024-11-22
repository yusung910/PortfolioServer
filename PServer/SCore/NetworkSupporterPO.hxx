/**
 *  @file NetworkSupporterPO.hxx
 *  @author YS
 *  @date 2024-10-16
 *  @project SCore
 *
 *  Winsock(����) ��ſ� �ʿ��� ���� ���� �Լ��� �����ϴ� Ŭ����
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
     *  ������ ���� �����Ǹ� ��ȯ �ϴ� �Լ�
     *  
     *      @param [in,out] _addr 
     */
    static void GetLocalIPAddress(std::vector<std::string>& _addr);
    /*!
     *  ip�� port�� ���ڰ����� ���޹޾Ƽ� 
     *
     *      @param [in] _ip   
     *      @param [in] _port 
     *
     *      @return The address info.
     */
    static SOCKADDR_IN GetAddressInfo(std::string _ip, int _port);
};

