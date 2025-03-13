#pragma once
#include "SummonsInfo.h"
#include <Position.h>
#include <IShape.h>

class Box;

class SummonsData
{
private:
    int m_nMapID = 0;
    EShape m_eShapeType = EShape::Point;
    float m_fPosX = 0.0f;
    float m_fPosY = 0.0f;
    float m_fRadius = 0.0f;
    float m_fWidth = 0.0f;
    float m_fHeight = 0.0f;
    float m_fRotation = 0.0f;
    bool m_bDirectional = false;

    Box* m_oBox = nullptr;
    std::vector<SummonsInfo> m_vSummonsInfo;
    std::vector<Position> m_vSummonsSpots;
    std::string m_sSummonsHelperName = "";

    float m_fSummonsSpotRadius = 1.0f;

    int m_nCurrentSummonOffset = 0;
    int m_nTotalSummonsCount = 0;

public:
    SummonsData();
    SummonsData(const SummonsData& _data);
    SummonsData(const SummonsData* _data);
    ~SummonsData();

    void SetPosX(const float& _x) noexcept;
    void SetPosY(const float& _y) noexcept;

    void Reset();

    const int& GetMapID() const noexcept;
    const EShape& GetShapeType() const noexcept;
    const float& GetPosX() const noexcept;
    const float& GetPosY() const noexcept;
    const float& GetRadius() const noexcept;
    const float& GetWidth() const noexcept;
    const float& GetHeight() const noexcept;
    const float& GetRotation() const noexcept;
    const bool& GetDirectional() const noexcept;
    const Box* GetBoxShape() const noexcept;

    void SetSummonsData(const EShape& _shape, const int& _mapID, const float& _posX, const float& _posY, const int& _rad, const float& _width, const float& _height, const float& _rot, const bool& _directional);
    void SetMapID(const int& _mapID) noexcept;
    void SetBoxShape(Box* _box) noexcept;
    void SetSummonsInfo(const int& _npcID, const int& _npcCount, const int& _resummonsTime, const MDBNPCSummons* _mdbSummons = nullptr);

    const std::vector<SummonsInfo>& GetSummonsInfoList() const;

    const std::string& GetSummonsHelperName() const;
    void SetSummonsHelperName(const std::string& _name);

    bool MakeBox();

    bool GetRandomPosition(Position& _pos);

};

