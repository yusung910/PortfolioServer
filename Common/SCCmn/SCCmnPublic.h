#pragma once
#pragma warning(disable : 4819)
#ifdef SERVER_BUILD

//서버 전용
#include <string>
#include <vector>
#else

//클라 전용

#endif // SERVER_BUILD


#include "SecureValueClass.h"

#include "GlobalConst.h"
#include "GlobalEnum.h"

#include "MasterDB.h"

//object stats, object stats manager 추가필요
//#include "ObjectStats.h"
//#include "ObjectStatsManager.h"

#include "DataProcessManager.h"

#include <Packet.h>