/**
 *  @file NetworkHostPoolPO.hxx
 *  @author YS
 *  @date 2024-10-29
 *  @project SCore
 *
 *  NetworkHost를 pool로 관리하기 위한 클래스
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
     *  HostID를 생성해서 m_oActiveMap에 HostID값과 NetworkHostPO를 할당
     *
     *      @return 
     */
    NetworkHostPO* Allocate();

    /*!
     *  인자값으로 전달 받은 NetworkHostPO를 m_oActiveMap에서 지우고 리셋한 뒤
     *  m_oFreeQueue에 추가한다
     *
     *      @param [in,out] _host 
     */
    void Release(NetworkHostPO* _host);

    /*!
     *  인자값으로 전달받은 HostID가 활성화 된 맵(m_oActiveMap)에 존재 여부를 반환한다
     *  true : 존재, false : 부재
     *      @param [in] _hostID 
     *
     *      @return 
     */
    bool Check(const int& _hostID);

    /*!
     *  인자값으로 전달받은 HostID으로 m_oActiveMap에 저장된 NetworkHostPO를 반환한다
     *
     *      @param [in] _hostID 
     *
     *      @return The host.
     */
    NetworkHostPO* GetHost(const int& _hostID);



};

