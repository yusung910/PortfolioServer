#include "PGPPrivate.h"
#include "InnerPacketStructures.h"

void LoginAccountProcessSelectDTO::Release()
{
    HeroExistServerList.clear();
}

void LoginAccountCharacterSelectDTO::Release()
{
    characterList.clear();
}
