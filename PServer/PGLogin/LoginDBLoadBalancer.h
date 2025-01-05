#pragma once
#include <DBServiceLoadBalancer.h>
#include <RefSingleton.h>

class LoginDBLoadBalancer : public DBServiceLoadBalancer, public RefSingleton<LoginDBLoadBalancer>
{
public:
    LoginDBLoadBalancer() = default;
    virtual ~LoginDBLoadBalancer() = default;
};

