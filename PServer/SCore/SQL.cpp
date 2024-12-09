#include "stdafx.hxx"
#include "SQL.hxx"

SQL::SQL()
{
    m_oSession = new DBSession();
}

SQL::~SQL()
{
    SafeDelete(m_oSession);
}

void SQL::Init()
{
    m_oSession->Init();
}
