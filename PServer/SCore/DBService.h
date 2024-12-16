#pragma once
#include "SCoreAPI.h"
#include "InnerPacket.h"

#include "DBWorker.h"
#include "StringUtil.h"

//���ǿ� ��ũ�� �Լ�

#define CheckSession()  auto lTmpSess = GetSession();                       \
                        if(nullptr == lTmpSess)                             \
                            return false;                                   \
                        Poco::Data::Session& lSess = *lTmpSess;             \

#define BEGIN_SESSION   try {
#define END_SESSION     }                                                   \
                        catch(Poco::Data::ODBC::StatementException& ex)     \
                        {                                                   \
                            VIEW_WRITE_ERROR("\n%s", StringUtil::UTF8_WSTR(ex.message().c_str()).c_str());  \
                        }                                                   \
                        catch(std::exception& e)                            \
                        {                                                   \
                            VIEW_WRITE_ERROR("\nDB Error : %s", e.what());  \
                        }
                        

//Statement�� �̿��� Select ��� ������ ���� ���� Ȯ��
#define HasResult(x) (false == x.toString().empty())

//Statement�� �̿��� Select �� �� ��� ������ ���� ���� üũ �� ���� ��� Break;
#define CheckResult(x) if(true == x.toString().empty()) break;

// ���ǿ� ��ũ�� �Լ� end

class DBWorker;
class DBServicePO;



class SCoreAPI DBService
{
private:
    DBServicePO* m_pDBServicePO = nullptr;

public:
    DBService();
    virtual ~DBService();

    void SetDBConfig(const std::string& _userID, const std::string _password, const std::string& _database, const std::string& _host, const std::string& _port);

    Poco::Data::Session* GetSession();

    void Exit();
    bool Push(InnerPacket::SharedPtr _data);

    int  GetServerID() const noexcept;
    void SetServerID(const int& _serverID) noexcept;

    bool CreateThread();
    size_t GetWorkQueueCount();
    virtual unsigned int Run();

    //void SetQuery(CSession* _session, TCHAR* _query);

private:
    void _RegisterHandler(const int& _protocolID, const std::function<bool(InnerPacket::SharedPtr)>& _pFunc);

protected:
    template<typename DispatcherType, typename EnumType, 
        typename = typename std::enable_if<std::is_base_of<DBService, DispatcherType>::value>::type>
    void RegisterHandler(const EnumType& _msgID, bool (DispatcherType::* _handler)(InnerPacket::SharedPtr))
    {
        if (nullptr == m_pDBServicePO)
            return;

        int lID = static_cast<int>(_msgID);
        DispatcherType* lDerived = static_cast<DispatcherType*>(this);

        auto lInvoker = [lDerived, _handler](InnerPacket::SharedPtr _packet)
            {
                if (nullptr == _packet.get())
                    return false;

                return (lDerived->*_handler)(_packet);
            };

        _RegisterHandler(lID, lInvoker);

    }
};

