#include "PGPPrivate.h"
#include "InnerPacketStructures.h"

void LoginAccountProcessSelectDTO::Release()
{
    HeroExistServerList.clear();
}

void LoginAccountPilgrimSelectDTO::Release()
{
    characterList.clear();
}
