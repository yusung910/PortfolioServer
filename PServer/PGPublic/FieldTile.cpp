#include "PGPPrivate.h"
#include "FieldTile.h"

#include <NetworkManager.h>
#include "PC.h"
#include "LoggingMacro.h"

#include "SYNCObject.h"
#include "SYNCObjectPool.h"

FieldTile::FieldTile(ObjectTag)
{
}

FieldTile::~FieldTile()
{
}

void FieldTile::Reset()
{
    FieldTileReset();
}

void FieldTile::FieldTileReset()
{
    m_nMapID = 0;

    m_oPCs.clear();

    m_vNPCs.clear();
    m_vNPCs.reserve(DEFULAT_OTHER_RESERVE_SIZE);

    m_vDropItems.clear();
    m_vDropItems.reserve(DEFULAT_OTHER_RESERVE_SIZE);

    m_vObjectBehaviors.clear();
    m_vNearTiles.clear();

    m_oTileLoc.Reset();

    for (auto& lSync : m_vObjectBehaviors)
    {
        if (nullptr == lSync)
            continue;

        if (0 == lSync->DecreaseSharedCount())
            SYNCObjectPool::GetInst().Push(lSync);

    }

}

void FieldTile::SetTileLocation(const int& _r, const int& _c) noexcept
{
    m_oTileLoc.Set(_r, _c);
}

void FieldTile::SetMapID(const int& _mapID) noexcept
{
    if(_mapID > 0) __LIKELY__
        m_nMapID = _mapID;
}
