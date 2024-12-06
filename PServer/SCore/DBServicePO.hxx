/**
 *  @file DBServicePO.hxx
 *  @author YS
 *  @date 2024-12-03
 *  @project SCore
 *
 *  Timer 클래스를 상속받은 클래스이기 떄문에
 *  주기적으로 실행할 함수 등록 가능
 */
#pragma once
#include <Windows.h>
#include <functional>
#include <vector>

#include "ObjectQueue.h"
#include "Queue"
#include "Timer.h"

#include "InnerPacket.h"
#include "DBWorker.h"

 //OLEDB를 이용한다!


class DBServicePO : public Timer
{
private:
    HANDLE m_hThread = INVALID_HANDLE_VALUE;
    bool m_ShouldExit = false;

    //내부패킷 처리
    ObjectQueue<InnerPacket::SharedPtr> m_oPacketQueue;
    std::unordered_map<int, std::function<bool(InnerPacket::SharedPtr)>> m_umHandlers;

    //스위칭 변수
    std::deque<std::shared_ptr<InnerPacket>> m_oWorkList;

    DBWorker m_oWorker;
    int m_nServerID = 0;

public:
    DBServicePO() = default;
    virtual ~DBServicePO() = default;

    void SetDBConfig(const std::string& _userID
        , const std::string& _password
        , const std::string& _database
        , const std::string& _host
        , const std::string& _port);

    CSession GetSession();

    void Exit();
    bool Push(InnerPacket::SharedPtr _data);

    const int& GetServerID() const noexcept;
    void SetServerID(const int& _serverID) noexcept;

    bool CreateThread();
    size_t GetWorkQueueCount();

    void SetQuery(const CSession* _session, TCHAR* _query);

    virtual unsigned int Run();

    void RegisterHandler(const int& _protocolID, const std::function<bool(InnerPacket::SharedPtr)>& _pFunc);

private:
    static unsigned int WINAPI ThreadFunc(void* _pSelf);

};

