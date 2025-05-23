#pragma once

/*!
 *  게임 내 맵(map)을 격자로 나눈 하나하나 요소를 Tile이라고 하며
 *  Tile을 계속해서 업데이트 하는 클래스.
 *  신규 Area가 있을 경우 상속 받아서 사용하게 될 클래스.
 */

#include "TileLocation.h"

class BaseObject;
class SYNCObject;

// NPC 주변에 PC가 없어도 업데이트 횟수
constexpr int NPC_UPDATE_COUNT_WITHOUT_PC = 2;

// 패킷 분할 전송 조건 (패킷 사이즈 오버 방지)
constexpr size_t SYNC_PACKET_DIVIDE_LIMIT_SIZE = 6000;

// 기본 cell Reserve Size
constexpr size_t DEFULAT_PC_RESERVE_SIZE = 20;
constexpr size_t DEFULAT_OTHER_RESERVE_SIZE = 20;



class FieldTile : public PGObject<FieldTile>
{
protected:
    int m_nMapID = 0;   // 맵 ID

    TileLocation m_oTileLoc;

    //Tile의 NPC 리스트
    std::vector<BaseObject*> m_vNPCs;

    //PC 리스트
    std::set<BaseObject*> m_oPCs;

    //std::vector<BaseObject*> m_vMinionCells;

    //드랍된 item리스트
    std::vector<BaseObject*> m_vDropItems;

    //싱크 Behaviors 리스트
    std::vector<SYNCObject*> m_vObjectBehaviors;

    //인접한 Tile 리스트
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

