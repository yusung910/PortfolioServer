#pragma once
#include <DBServiceLoadBalancer.h>
#include <RefSingleton.h>
#include "GameDBService.h"

class GameDBLoadBalancer : public DBServiceLoadBalancer, public RefSingleton< GameDBLoadBalancer>
{
public:
    GameDBLoadBalancer() = default;
    virtual ~GameDBLoadBalancer() = default;
};