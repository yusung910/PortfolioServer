#pragma once
#include <atldbcli.h>  
#include <vector>
#include <string>


class DBStatements;

typedef CCommand<CDynamicParameterAccessor, CRowset, CMultipleResults>    CxParamCmd;

class DBSession
{
private:
	std::string		m_sConnection = "";
	CDataSource		m_ds;
	CSession		m_oSession;
	CxParamCmd		m_oCmd;

    DBStatements* m_oStatements = nullptr;

public:
	DBSession();
	~DBSession();

	void Init(std::string _connectionStr);
	bool IsConnected() const;

    inline DBStatements& operator << (const char* _query)
    {
        void* localpDummy;

        //Create
        HRESULT localHR = m_oCmd.Create(m_oSession, _query);
        if (FAILED(localHR))
        {
            AtlTraceErrorRecords(localHR);
        }

        localHR = m_oCmd.Prepare();
        if (FAILED(localHR))
        {
            AtlTraceErrorRecords(localHR);
        }

        //쿼리 실행에 필요한 파라미터를 세팅
        localHR = m_oCmd.BindParameters(&m_oCmd.m_hParameterAccessor, m_oCmd.m_spCommand, &localpDummy);

        if (FAILED(localHR))
        {
            AtlTraceErrorRecords(localHR);
        }

        m_oStatements->SetHRslt(localHR);

        return *m_oStatements;
    }

    inline DBStatements& operator , (const char* _t)
    {
    }

    template <typename T>
    inline HRESULT SetParam(const ULONG _num, const T& _data)
    {
        m_oCmd.SetParam(_num, _data);
    };

    HRESULT ExecuteQuery();

    template <typename T>
    inline HRESULT SetOut(T& _data)
    {
        m_oHr = m_oCmd.GetNextResult(_data);
    }

    template <typename... Args>
    inline HRESULT SetResults(std::vector<std::tuple<Args...>> &_datas)
    {
        int argsCnt = std::tuple_size<decltype(_datas)>::value;
        while (m_oHr == S_OK)
        {
            std::tuple<Args...> localTmp;
            //_datas.push_back();
            for (int i = 0; i < argsCnt; i++)
            {
                std::get<i>(_datas) = m_oCmd.GetValue(i);
            }
            _datas.push_back(localTmp);
            m_oHr = m_oCmd.MoveNext();
        }
    }

private:
	void _ConnectDB();
};