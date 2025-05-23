#pragma once

#include <Position.h>

struct TileLocation;

class Area;
class FieldTile;

class BaseObject;

class PC;
class SYNCObjectDUserBuff;

constexpr int TILES_SIZE = 20;

class Field
{
protected:
    // 2차원 배열로 나열되어 있는 각각의 Field의 Tile들이
    // 1차원 배열 형태로 저장되는 변수
    std::vector<std::unique_ptr<FieldTile>> m_oFieldTiles;
    Position m_oStartPos;

    int m_nMaxTilesRow = 0;
    int m_nMaxTilesCol = 0;
    int m_nMapID = 0;

    int m_nTileSize = TILES_SIZE;

    EMap::Type m_eMapType = EMap::Type::None;


    int m_nChannelSeq = 0;
    int m_nChannelPCCount = 0;
    int m_nChannelPCCountLimit = 0;

    std::unordered_set<int> m_usChannelUserHostIDs;

    //인접 Tiles
    std::unordered_map<FieldTile*, std::unordered_set<FieldTile*>> m_umNearTiles;

    //std::vector<int> m_oWorkSendIDs; //이게 왜 필요할까??
protected:
    FieldTile* GetFieldTile(const Position& _pos) const;
    FieldTile* GetFieldTile(const int& _r, const int& _c) const;

    int GetTileIndexByPosition(const Position& _pos) const;
    int CalculrateTileIndex(const int& _r, const int& _c) const;

    void _GetNearFieldTiles(TileLocation& _minLoc, TileLocation& _maxLoc, std::unordered_set<const FieldTile*>& _FieldTiles) const;
    void _GetNearFieldTiles(TileLocation& _minLoc, TileLocation& _maxLoc, std::unordered_set<FieldTile*>& _FieldTiles);

public:
    Field(const int& _mapID = 0);
    void SetMapID(const int& _mapID) noexcept;

    void Reset();
    virtual void Create(const int& _w, const int& _h, const Position& _startPos);

    void CalculateNearFieldTileLocation(const FieldTile* _fieldTile, TileLocation& _minField, TileLocation& _maxField);

};

