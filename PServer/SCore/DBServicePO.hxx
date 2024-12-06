/**
 *  @file DBServicePO.hxx
 *  @author YS
 *  @date 2024-12-03
 *  @project SCore
 *
 *  Timer Ŭ������ ��ӹ��� Ŭ�����̱� ������
 *  �ֱ������� ������ �Լ� ��� ����
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

 //OLEDB�� �̿��Ѵ�!


class DBServicePO : public Timer
{
private:
    HANDLE m_hThread = INVALID_HANDLE_VALUE;
    bool m_ShouldExit = false;

    //������Ŷ ó��
    ObjectQueue<InnerPacket::SharedPtr> m_oPacketQueue;
    std::unordered_map<int, std::function<bool(InnerPacket::SharedPtr)>> m_umHandlers;

    //����Ī ����
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

