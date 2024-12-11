#pragma once
#include <RefSingleton.h>

class DBStatements : public RefSingleton<DBStatements>
{
private:
    HRESULT m_oHr = S_FALSE;
public:
    DBStatements(HRESULT _hr);
    ~DBStatements();

    void SetHRslt(HRESULT _hr);

    bool IsConnected();
};

