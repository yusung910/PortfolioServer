#include "PGPPrivate.h"
#include "InnerPacketStructures.h"

void spLoginAccountProcessSelectDTO::Release()
{
    PilgrimExistServerList.clear();
}

void spLoginAccountPilgrimSelectDTO::Release()
{
    PligrimList.clear();
}