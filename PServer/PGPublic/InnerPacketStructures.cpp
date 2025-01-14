#include "PGPPrivate.h"
#include "InnerPacketStructures.h"

void pLoginAccountResult::Release()
{
    HeroExistServerList.clear();
}

void pLoginAccountCharacterSelect::Release()
{
    characterList.clear();
}
