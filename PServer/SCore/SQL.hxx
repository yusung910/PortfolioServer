#pragma once
#include "DBSession.hxx"

class DBSession;

class SQL
{
private:
    DBSession* m_oSession = nullptr;
public:
    SQL();
    ~SQL();

    void Init();

    template <typename T>
    HRESULT operator << (const T& _t)
    {
        return m_oSession->SetQuery(_t);
    }
    
};

