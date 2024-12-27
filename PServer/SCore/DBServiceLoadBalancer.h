#pragma once
#include "SCoreAPI.h"
#include <string>
#include "DBService.h"

class DBServiceLoadBalancerMap;

class SCoreAPI DBServiceLoadBalancer
{
private:
    size_t m_nServiceCount = 0;     //총 DB 동시 실행 갯수(로드 밸런신 전용)

    /*!
     *  메인 DB를 사용하는 쓰레드를 n개 생성하고
     *  작업을 분배하여 동시에 DB를 처리하는 방식.
     *  SetDBConfig()로 등록, Push()로 패킷 생성하여 분배
     */
    DBService** m_pServiceList = nullptr;   //로드 밸런싱 할 서비스 목록

    /*!
     *  메인 DB를 로드밸런싱 하지 않고 내부 로직 전용으로 사용하게 될 변수
     */
    DBService* m_pDirectService = nullptr; //서버 단일 서비스

    /*!
     *  여러 DB에 1 쓰레드씩 물려서 사용하는 경우 사용됨.
     */
    DBServiceLoadBalancerMap* m_pServerIDServerList = nullptr;

    bool m_bInitialized = false;

public:
    DBServiceLoadBalancer();
    ~DBServiceLoadBalancer();

    /*!
     *  Pushes the service load balancer.
     *
     *      @param [in] _data
     *
     *      @return
     */
    bool Push(InnerPacket::SharedPtr _data);

    bool PushByDBID(InnerPacket::SharedPtr _data, const int& _serverID);

    bool PushDirect(InnerPacket::SharedPtr _data);

    void Exit();

    /*!
     *  DB 연결을 설정하고 DBService 쓰레드를 생성 할당한다.
     *
     *      @tparam T
     *      @tparam
     *
     *      @param [in] _id
     *      @param [in] _password
     *      @param [in] _database
     *      @param [in] _host
     *      @param [in] _port
     *      @param [in] _workerCount
     */
    template <typename T, typename = typename std::enable_if<std::is_base_of<DBService, T>::value>::type>
    void SetDBConfig(const std::string& _id, const std::string& _password, const std::string& _database, const std::string& _host, const std::string& _port, const size_t& _workerCount)
    {
        if (true == m_bInitialized)
            return;

        if (_workerCount <= 0)
            return;

        if (nullptr != m_pServiceList)
            return;

        m_nServiceCount = _workerCount;

        m_pDirectService = new T();

        m_pDirectService->SetDBConfig(_id, _password, _database, _host, _port);
        if (false == m_pDirectService->CreateThread())
        {
            VIEW_WRITE_ERROR("DB Service CreateThread() Failed - (%s:%s)[%s] ", _host.c_str(), _port.c_str(), _database.c_str());
            SafeDelete(m_pDirectService);
            return;
        }

        m_pServiceList = new DBService * [m_nServiceCount];

        //-----------------------------------------
        //해당 라인 기준으로 for반복문이 나뉘어 있음
        //동일한 반복문인데 왜 나뉘어 있는지 테스트 필요
        for (size_t i = 0; i < m_nServiceCount; ++i)
        {
            m_pServiceList[i] = new T();
            //-----------------------------------------
            m_pServiceList[i]->SetDBConfig(_id, _password, _database, _host, _port);
            if (false == m_pServiceList[i]->CreateThread())
            {
                VIEW_WRITE_ERROR("DB Service(%d) CreateThread() Failed - (%s:%s)[%s] ", i, _host.c_str(), _port.c_str(), _database.c_str());

                m_pDirectService->Exit();
                SafeDelete(m_pDirectService);

                for (size_t j = 0; j < m_nServiceCount; ++j)
                {
                    m_pServiceList[j]->Exit();
                    SafeDelete(m_pServiceList[j]);
                }
                SafeDeleteArray(m_pServiceList);
                return;
            }
        }
        VIEW_INFO("DB Service Successfully Initialized. (%s:%s)[%s]", _host.c_str(), _port.c_str(), _database.c_str());

        m_bInitialized = true;
    }

    /*!
     *  DBConfig에 해당하는 Service를 추가하고
     *  DBService의 쓰레드를 실행한다
     *
     *      @tparam T
     *      @tparam
     *
     *      @param [in] _id
     *      @param [in] _password
     *      @param [in] _database
     *      @param [in] _host
     *      @param [in] _port
     *      @param [in] _serverID
     *
     *      @return
     */
    template <typename T, typename = typename std::enable_if<std::is_base_of<DBService, T>::value>::type>
    bool AddDBConfig(const std::string& _id, const std::string& _password, const std::string& _database, const std::string& _host, const std::string& _port, const int& _serverID)
    {
        if (true == _IsExistsServerID(_serverID))
        {
            VIEW_INFO("DB Service By ServerID(%d) is Exists. (%s:%s)", _serverID, _host.c_str(), _port.c_str());
            return false;
        }

        T* lpService = new T();
        lpService->SetDBConfig(_id, _password, _database, _host, _port);
        lpService->SetServerID(_id);

        if (false == lpService->CreateThread())
        {
            VIEW_INFO("DB Service By ServerID(%d) has CreateThread() Fail. (%s:%s)[]", _serverID, _host.c_str(), _port.c_str(), _database.c_str());
            SafeDelete(lpService);
            return false;
        }

        if (false == _AddDBService(_serverID, lpService))
        {
            VIEW_INFO("DB Service By ServerID(%d) has _AddDBService() Fail. (%s:%s)[]", _serverID, _host.c_str(), _port.c_str(), _database.c_str());
            SafeDelete(lpService);
            return false;
        }

        m_bInitialized = true;

        return true;
    }

    template <typename T, typename = typename std::enable_if<std::is_base_of<DBService, T>::value>::type>
    T* GetDirectService()
    {
        return dynamic_cast<T*>(m_pDirectService);
    }

    template <typename T, typename = typename std::enable_if<std::is_base_of<DBService, T>::value>::type>
    T* GetServerIDServiceList()
    {
        return dynamic_cast<T*>(m_pServerIDServerList);
    }

private:
    bool _AddDBService(const int& _serverID, DBService* _service);
    bool _IsExistsServerID(const int& _serverID);
};

