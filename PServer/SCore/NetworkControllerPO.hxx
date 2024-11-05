/**
 *  @file NetworkControllerPO.hxx
 *  @author YS
 *  @date 2024-10-29
 *  @project SCore
 *
 *  IOCP 통신을 위한 WSASocket 관련 기능들을 관리하는 클래스
 */
#pragma once

#include "Timer.h"
#include "BlockingQueue.h"
#include "SConnectorTargetInfo.h"


class NetworkHostPO;
class NetworkContextPO;

class NetworkControllerPO : public Timer
{
private:
    volatile bool m_bIsTerminated;
    HANDLE m_hNetworkControl = INVALID_HANDLE_VALUE;
    BlockingQueue<NetworkContextPO> m_oMsgQueue;

    std::mutex m_xRecvLock;
    std::mutex m_xHostIDLock;

    std::unordered_map<int, NetworkHostPO*> m_oHostList;
    std::vector<ConnectorTargetInfo> m_ConnectorHostList;

public:
    NetworkControllerPO();
    virtual ~NetworkControllerPO();

public:
    /*!
     *  쓰레드 생성
     *
     *      @return 
     */
    bool CreateThread();
    void TerminateThread();

    static unsigned int WINAPI ExcuteThread(void* _arg);


private:
    /*!
     *  인자값으로 전달받은 NetworkHostPO에 Socket을 생성하고
     *  멤버 변수 m_oHostList 맵에 NetworkHostPO를 등록하며
     *  그 후 Socket 통신을 위해 NetworkWorker에 등록하는 함수
     *
     *      @param [in,out] _host 
     *
     *      @return 
     */
    bool _AddHost(NetworkHostPO* _host);

    /*!
     *  인자값으로 전달받은 hostID로
     *  클래스 멤버변수 m_oHostList에 저장되어 있는 NetworkHostPO를 찾아 반환한다
     *
     *      @param [in] _hostID
     *
     *      @return
     */
    NetworkHostPO* _FindHost(int _hostID);

    /*!
     *  m_oHostList에 등록되어 있는 NetworkHostPO의 IsAlive() 값이 true일 경우
     *  Update함수를 지속적으로 실행하고
     *  false일 경우 해당 NetworkHostPO를 지운다
     */
    void _UpdateHost();


    /*!
     *  인자값으로 전달받은 NetworkHostPO에 저장되어 있는 데이터들을
     *  ConnectorTargetInfo 구조체를 생성하고 저장해서
     *  m_ConnectorHostList에 push_back한다
     *
     *      @param [in,out] _host 
     */
    void _AddConnectorHost(NetworkHostPO* _host);

    /*!
     *  인자값으로 전달받은 hostID 값으로 m_ConnectorHostList에 저장되어 있는 ConnectorTargetInfo을 찾아서 지운다
     *
     *      @param [in] _hostID 
     */
    void _RemoveConnectorHost(int _hostID);

private:

    void ProcessThread();
};

