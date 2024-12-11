#include "stdafx.hxx"
#include "DBStatements.h"

DBStatements::DBStatements(HRESULT _hr)
    :m_oHr(_hr)
{
}

DBStatements::~DBStatements()
{
}

void DBStatements::SetHRslt(HRESULT _hr)
{
    m_oHr = _hr;
}

bool DBStatements::IsConnected()
{
    return !FAILED(m_oHr);
}
