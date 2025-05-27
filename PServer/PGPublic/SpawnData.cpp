#include "PGPPrivate.h"
#include "SpawnData.h"
#include <Box.h>
#include "Navi.h"
#include <CmnMath.h>

SpawnData::SpawnData()
{
    Reset();
}

SpawnData::SpawnData(const SpawnData& _data)
{
    m_nMapID = _data.m_nMapID;
    m_eShapeType = _data.m_eShapeType;
    m_fPosX = _data.m_fPosX;
    m_fPosY = _data.m_fPosY;
    m_nRadius = _data.m_nRadius;
    m_fWidth = _data.m_fWidth;
    m_fHeight = m_fHeight;
    m_fRotation = m_fRotation;

    m_vSpawnInfo.assign(_data.m_vSpawnInfo.begin(), _data.m_vSpawnInfo.end());
    if (nullptr != _data.m_oBox)
    {
        m_oBox = new Box(*_data.m_oBox);
    }

    m_sSummonsHelperName.assign(_data.m_sSummonsHelperName.begin(), _data.m_sSummonsHelperName.end());
    m_vSummonsSpots.assign(_data.m_vSummonsSpots.begin(), _data.m_vSummonsSpots.end());

    m_fSummonsSpotRadius = _data.m_fSummonsSpotRadius;

    m_nCurrentSummonOffset = _data.m_nCurrentSummonOffset;
    m_nTotalSummonsCount = _data.m_nTotalSummonsCount;
}

SpawnData::SpawnData(const SpawnData* _data)
{
    m_nMapID = _data->m_nMapID;
    m_eShapeType = _data->m_eShapeType;
    m_fPosX = _data->m_fPosX;
    m_fPosY = _data->m_fPosY;
    m_nRadius = _data->m_nRadius;
    m_fWidth = _data->m_fWidth;
    m_fHeight = m_fHeight;
    m_fRotation = m_fRotation;

    m_vSpawnInfo.assign(_data->m_vSpawnInfo.begin(), _data->m_vSpawnInfo.end());
    if (nullptr != _data->m_oBox)
    {
        m_oBox = new Box(*_data->m_oBox);
    }

    m_sSummonsHelperName.assign(_data->m_sSummonsHelperName.begin(), _data->m_sSummonsHelperName.end());
    m_vSummonsSpots.assign(_data->m_vSummonsSpots.begin(), _data->m_vSummonsSpots.end());

    m_fSummonsSpotRadius = _data->m_fSummonsSpotRadius;

    m_nCurrentSummonOffset = _data->m_nCurrentSummonOffset;
    m_nTotalSummonsCount = _data->m_nTotalSummonsCount;
}

SpawnData::~SpawnData()
{
    Reset();
}

void SpawnData::SetPosX(const float& _x) noexcept
{
    m_fPosX = _x;
}

void SpawnData::SetPosY(const float& _y) noexcept
{
    m_fPosY = _y;
}

void SpawnData::Reset()
{
    m_nMapID = 0;
    m_eShapeType = EShape::Point;
    m_fPosX = 0.f;
    m_fPosY = 0.f;
    m_nRadius = 0;
    m_fWidth = 0.f;
    m_fHeight = 0.f;
    m_fRotation = 0;

    m_vSpawnInfo.clear();

    SafeDelete(m_oBox);

    m_vSummonsSpots.clear();

    m_fSummonsSpotRadius = 1.0f;

    m_nCurrentSummonOffset = 0;
    m_nTotalSummonsCount = 0;
}

const int& SpawnData::GetMapID() const noexcept
{
    return m_nMapID;
}

const EShape& SpawnData::GetShapeType() const noexcept
{
    return m_eShapeType;
}

const float& SpawnData::GetPosX() const noexcept
{
    return m_fPosX;
}

const float& SpawnData::GetPosY() const noexcept
{
    return m_fPosY;
}

const int& SpawnData::GetRadius() const noexcept
{
    return m_nRadius;
}

const float& SpawnData::GetWidth() const noexcept
{
    return m_fWidth;
}

const float& SpawnData::GetHeight() const noexcept
{
    return m_fHeight;
}

const float& SpawnData::GetRotation() const noexcept
{
    return m_fRotation;
}

const bool& SpawnData::GetDirectional() const noexcept
{
    return m_bDirectional;
}

const Box* SpawnData::GetBoxShape() const noexcept
{
    return m_oBox;
}

void SpawnData::SetSpawnData(const EShape& _shape, const int& _mapID, const float& _posX, const float& _posY, const int& _rad, const float& _width, const float& _height, const float& _rot, const bool& _directional)
{
    m_eShapeType = _shape;
    m_nMapID = _mapID;
    m_fPosX = _posX;
    m_fPosY = _posY;
    m_nRadius = _rad;

    m_fWidth = _width;
    m_fHeight = _height;
    m_fRotation = _rot;
    m_bDirectional = _directional;
}

void SpawnData::SetMapID(const int& _mapID) noexcept
{
    m_nMapID = _mapID;
}

void SpawnData::SetBoxShape(Box* _box) noexcept
{
    SafeDelete(m_oBox);
    m_oBox = _box;
}

void SpawnData::SetSpawnInfo(const int& _npcID, const int& _npcCount, const int& _respawnTime, const int& _spawnGroupSeq, const MDBNPCSpawn* _mdbspawn)
{
    SpawnInfo lInfo{ _npcID, _npcCount, _respawnTime, _spawnGroupSeq, _mdbspawn };
    m_vSpawnInfo.push_back(lInfo);
}

const std::vector<SpawnInfo>& SpawnData::GetSpawnInfoList() const
{
    return m_vSpawnInfo;
}

const std::string& SpawnData::GetSummonsHelperName() const
{
    return m_sSummonsHelperName;
}

void SpawnData::SetSummonsHelperName(const std::string& _name)
{
    m_sSummonsHelperName = _name;
}

bool SpawnData::MakeBox()
{
    if (nullptr != m_oBox)
        return false;

    float lRotArrange = 180.0f - GetRotation();

    m_oBox = new Box();

    if (nullptr == m_oBox)
        return false;

    m_oBox->SetPosition(Position(GetPosX(), GetPosY()));
    m_oBox->SetWidth(GetWidth() * 0.5f);
    m_oBox->SetHeight(GetHeight() * 0.5f);
    m_oBox->SetRotation(lRotArrange);
    m_oBox->MakeData();

    if (false == m_oBox->IsValid())
    {
        SafeDelete(m_oBox);
        return false;
    }

    return true;
}

bool SpawnData::GetRandomPosition(Position& _pos)
{
    switch(m_eShapeType)
    {
    case EShape::Point:
    {
        _pos.Set(m_fPosX, m_fPosY);
    }
    break;

    case EShape::Circle:
    case EShape::Box:
    {
        bool lRet = false;

        if (true == m_vSummonsSpots.empty())
        {
            for (int i = 0; i < 10; i++)
            {
                _pos.Set(m_fPosX, m_fPosY);

            }
        }
        else
        {
        }

        return lRet;
    }
    break;
    default:
        return false;
    }

    return true;
}


