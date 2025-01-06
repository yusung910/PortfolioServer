#include "PGPPrivate.h"
#include "InnerPacketStructures.h"

void pLoginAuthResult::Release()
{
    HeroExistServerList.clear();
}
