#include "PGPPrivate.h"
#include "Navi.h"
#include "Random.h"
#include "StringUtil.h"

#include <DetourCommon.h>
#include <MathUtil.h>
#include <Clock.h>

Navi::Navi()
{
}

Navi::~Navi()
{
    for (auto lIt : m_umZoneMeshList)
    {
        if (nullptr != lIt.second)
            SafeDelete(lIt.second);
    }

    m_umZoneMeshList.clear();
}

bool Navi::Init(std::vector<MDBMapInfo*>* _mapInfo)
{
    if (true == m_bIsInit)
        return false;

    if (nullptr == _mapInfo)
        return false;

    int lSuccessCount = 0;

    int64_t lStartTime = Clock::GetTick64();
    
    for (auto lIter : *_mapInfo)
    {
        int lMeshSize = std::max(lIter->MapHeight, lIter->MapWidth);

    }

    return false;
}
