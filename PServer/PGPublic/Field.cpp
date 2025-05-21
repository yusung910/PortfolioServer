#include "PGPPrivate.h"
#include "Field.h"
#include "FieldTile.h"

#include "Clock.h"
#include "PC.h"
#include "MDBDataManager.h"

#include <NetworkManager.h>

FieldTile* Field::GetFieldTile(const Position& _pos) const
{
    return nullptr;
}

FieldTile* Field::GetFieldTile(const int& _r, const int& _c) const
{
    return nullptr;
}

int Field::CalculrateTileIndex(const int& _r, const int& _c) const
{

    return 0;
}

Field::Field(const int& _mapID)
{
    m_nMapID = _mapID;
}

void Field::SetMapID(const int& _mapID) noexcept
{
    m_nMapID = _mapID;
    auto localMapData = MDBDataManager::GetInst().GetMapInfo(_mapID);

    if (nullptr == localMapData)
    {
        m_eMapType = localMapData->GetMapType();
    }
    else
    {
        m_eMapType = EMap::Type::None;
    }
}

void Field::Reset()
{
    m_oFieldTiles.clear();

    //shrink_to_fit() : vector�� ��� �ϴ� �޸𸮸� ����ȭ�Ͽ� �����ϴ� �Լ�
    m_oFieldTiles.shrink_to_fit();

    m_nMaxTilesRow = m_nMaxTilesCol = 0;

    m_nChannelPCCountLimit = MAX_USER_CNT_CHANNEL;
}

void Field::Create(const int& _w, const int& _h, const Position& _startPos)
{
    //�� ����� tile������� ������ Row X Col�� ���� ���Ѵ�
    //ex) 100*100 �������� ���� 20 ������(20)������� ���� ���
    // 5X5�� Ÿ���� �ȴ�.
    int localSizeRow = (m_nTileSize == 0) ? 0 : _w / m_nTileSize;
    int localSizeCol = (m_nTileSize == 0) ? 0 : _h / m_nTileSize;

    m_nMaxTilesRow = std::max(localSizeRow, 0);
    m_nMaxTilesCol = std::max(localSizeCol, 0);

    //Row X Col ��ŭ�� �迭 ����(5x5 -> 25)
    m_oFieldTiles.reserve(size_t(m_nMaxTilesRow) * size_t(m_nMaxTilesCol));
    
    m_oStartPos = _startPos;

    for (int r = 0; r < localSizeRow; r++)
    {
        for (int c = 0; c < localSizeCol; c++)
        {
            FieldTile::UniquePtr tile = FieldTile::New();
            tile->SetTileLocation(r, c);
            tile->SetMapID(m_nMapID);

            std::unique_ptr<FieldTile> localTile(std::move(tile.release()));
            m_oFieldTiles.push_back(std::move(localTile));
        }
    }

    std::unordered_set<FieldTile*> localUpdateTiles;
    localUpdateTiles.reserve(G_N_MAX_SYNC_ENTRY_COUNT);

    TileLocation min, max;

    m_umNearTiles.clear();

    for (auto lIter = m_oFieldTiles.begin(); lIter != m_oFieldTiles.end(); lIter++)
    {

    }
}
