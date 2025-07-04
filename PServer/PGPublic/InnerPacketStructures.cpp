#include "PGPPrivate.h"
#include "InnerPacketStructures.h"

void spLoginAccountProcessSelectDTO::Release()
{
    m_usPilgrimExistServerList.clear();
}

void spLoginAccountPilgrimSelectDTO::Release()
{
    PligrimList.clear();
}