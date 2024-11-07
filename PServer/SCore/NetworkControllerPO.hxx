/**
 *  @file NetworkControllerPO.hxx
 *  @author YS
 *  @date 2024-10-29
 *  @project SCore
 *  
 *  NetworkWorker와 NetworkHost를 통해 데이터를 세팅하며
 *  IOCP 통신을 하기 위한 클래스.
 *  EContextType에 따라 IOCP 통신을 한다
 *  
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
     *  NetworkController 쓰레드 핸들 생성
     *
     *      @return 
     */
    bool CreateThread();

    /*!
     *  NetworkController 쓰레드 중단
     */
    void TerminateThread();

    /*!
     *  CreateThread()에 생성된 쓰레드 핸들에서 실행할 함수
     *
     *      @param [in,out] _arg 
     *
     *      @return 
     */
    static unsigned int WINAPI ExcuteThread(void* _arg);

    /*!
     *  NetworkController에서 처리할 NetworkContextPO를 넣는 함수
     *
     *      @param [in,out] _ctxt 
     *
     *      @return 
     */
    bool PushThread(NetworkContextPO* _ctxt);

    /*!
     *  m_ConnectorHostList에 저장된 데이터 중
     *  인자값으로 전달받은 ip와 port 값과 일치하는 HostID값을 반홚나다
     *
     *      @param [in] _ip   
     *      @param [in] _port 
     *
     *      @return HostID
     */
    int GetConnectorHostID(const std::string& _ip, int _port);

    /*!
     *  인자값으로 전달받은 HostID에 Packet을 전송
     *
     *      @param [in] _hostID 
     *      @param [in] _packet 
     *
     *      @return 
     */
    bool SendPacketToHost(const int& _hostID, Packet::SharedPtr _packet);


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
    /*!
     *  m_oMsgQueue에 저장된 NetworkContextPO 데이터를 ContextType에 맞게 처리하는 함수
     *  ExcuteThread() 함수 내부에서 실행 되며
     *  EContextType(Connect, Listen, Join, Close) 값에 해당하는 함수가 실행된다
     *  각각 함수에 m_oMsgQueue에 저장된 NetworkContextPO Pointer를 인자 값으로 전달하여 실행한다
     */
    void ProcessThread();

    /*!
     *  인자 값으로 전달받은 NetworkContextPO를 소켓 통신하기 위해 ConnectEx함수로
     *  소켓을 연결하는 함수
     *  
     *      @param [in,out] _ctxt 
     */
    void ProcessConnect(NetworkContextPO& _ctxt);

    /*!
     *  인자 값으로 전달받은 NetworkContextPO의
     *  NetworkHostPO를 Listen 상태로 변경하는 함수
     *
     *      @param [in,out] _ctxt 
     */
    void ProcessListen(NetworkContextPO& _ctxt);

    /*!
     *  인자 값으로 전달받은 NetworkContextPO의 NetworkHostPO에서 WSARecv() 함수를 통해
     *  Socket 데이터를 수신 처리한다
     *  
     *      @param [in,out] _ctxt 
     */
    void ProcessJoin(NetworkContextPO& _ctxt);

    /*!
     *  인자 값으로 전달받은 NetworkContextPO의 NetworkHostPO를 Socket Close 처리한다
     *
     *      @param [in,out] _ctxt 
     */
    void ProcessClose(NetworkContextPO& _ctxt);
};

