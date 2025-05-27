#pragma once

#include <DBService.h>

class UserDBService : public DBService
{
private:
    int m_nCurrentServerID = 1;
public:
    UserDBService();
    virtual ~UserDBService();
};

