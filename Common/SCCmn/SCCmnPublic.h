#pragma once

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

#include "DataProcessManager.h"

#include <Packet.h>