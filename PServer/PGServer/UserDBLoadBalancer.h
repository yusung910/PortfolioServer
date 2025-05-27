#pragma once
#include <DBServiceLoadBalancer.h>
#include <RefSingleton.h>

#include "UserDBService.h"

class UserDBLoadBalancer : public DBServiceLoadBalancer, RefSingleton<UserDBLoadBalancer>
{
    UserDBLoadBalancer() = default;
    virtual ~UserDBLoadBalancer() = default;

};