#pragma once
#include "SCoreAPI.h"
#include <string>
#include "DBService.h"

class DBServiceLoadBalancerMap;

class SCoreAPI DBServiceLoadBalancer
{
private:
    size_t m_nServiceCount = 0;     //�� DB ���� ���� ����(�ε� �뷱�� ����)

    /*!
     *  ���� DB�� ����ϴ� �����带 n�� �����ϰ�
     *  �۾��� �й��Ͽ� ���ÿ� DB�� ó���ϴ� ���.
     *  SetDBConfig()�� ���, Push()�� ��Ŷ �����Ͽ� �й�
     */
    DBService** m_pServiceList = nullptr;   //�ε� �뷱�� �� ���� ���

    /*!
     *  ���� DB�� �ε�뷱�� ���� �ʰ� ���� ���� �������� ����ϰ� �� ����
     */
    DBService* m_pDirectService = nullptr; //���� ���� ����

    /*!
     *  ���� DB�� 1 �����徿 ������ ����ϴ� ��� ����.
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
     *  DB ������ �����ϰ� DBService �����带 ���� �Ҵ��Ѵ�.
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
        //�ش� ���� �������� for�ݺ����� ������ ����
        //������ �ݺ����ε� �� ������ �ִ��� �׽�Ʈ �ʿ�
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
     *  DBConfig�� �ش��ϴ� Service�� �߰��ϰ�
     *  DBService�� �����带 �����Ѵ�
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

