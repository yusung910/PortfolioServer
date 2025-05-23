#pragma once

/*!
 *  ���� �� ��(map)�� ���ڷ� ���� �ϳ��ϳ� ��Ҹ� Tile�̶�� �ϸ�
 *  Tile�� ����ؼ� ������Ʈ �ϴ� Ŭ����.
 *  �ű� Area�� ���� ��� ��� �޾Ƽ� ����ϰ� �� Ŭ����.
 */

#include "TileLocation.h"

class BaseObject;
class SYNCObject;

// NPC �ֺ��� PC�� ��� ������Ʈ Ƚ��
constexpr int NPC_UPDATE_COUNT_WITHOUT_PC = 2;

// ��Ŷ ���� ���� ���� (��Ŷ ������ ���� ����)
constexpr size_t SYNC_PACKET_DIVIDE_LIMIT_SIZE = 6000;

// �⺻ cell Reserve Size
constexpr size_t DEFULAT_PC_RESERVE_SIZE = 20;
constexpr size_t DEFULAT_OTHER_RESERVE_SIZE = 20;



class FieldTile : public PGObject<FieldTile>
{
protected:
    int m_nMapID = 0;   // �� ID

    TileLocation m_oTileLoc;

    //Tile�� NPC ����Ʈ
    std::vector<BaseObject*> m_vNPCs;

    //PC ����Ʈ
    std::set<BaseObject*> m_oPCs;

    //std::vector<BaseObject*> m_vMinionCells;

    //����� item����Ʈ
    std::vector<BaseObject*> m_vDropItems;

    //��ũ Behaviors ����Ʈ
    std::vector<SYNCObject*> m_vObjectBehaviors;

    //������ Tile ����Ʈ
    std::vector<FieldTile*> m_vNearTiles;

    int m_nNPCUpdateCountWithoutPC = 0;

public:
    FieldTile(ObjectTag);
    virtual ~FieldTile();

    virtual void Reset();
    void FieldTileReset();

    void SetTileLocation(const int& _r, const int& _c) noexcept;
    const TileLocation& GetLocation() const noexcept;

    void SetMapID(const int& _mapID) noexcept;

    void AddNearTileData(FieldTile* _fieldData);
};

