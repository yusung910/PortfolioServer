#include "PGPPrivate.h"
#include "InnerPacketStructures.h"

void LoginAccountProcessSelectDTO::Release()
{
    PilgrimExistServerList.clear();
}

void LoginAccountPilgrimSelectDTO::Release()
{
    PligrimList.clear();
}