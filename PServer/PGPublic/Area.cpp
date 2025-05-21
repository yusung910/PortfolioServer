#include "PGPPrivate.h"
#include "Area.h"

#include <NetworkManager.h>

#include "MDBDataManager.h"

#include "Field.h"
#include "FieldTile.h"
#include "BaseObject.h";
#include "PC.h";
#include "SYNCObjectDUserBuff.h";

Area::Area(ObjectTag)
{

}

Area::~Area()
{
    SafeDelete(m_pField);
}

void Area::Reset()
{
    if (nullptr != m_pField)
    {
        for (size_t i = 0; i < m_nChannelCount; i++)
        {
            m_pField[i].Reset();
        }
    }
}

void Area::SetMapID(const int& _mapID) noexcept
{
    m_nMapID = _mapID;
}

const int& Area::GetMapID() const noexcept
{
    // TODO: 여기에 return 문을 삽입합니다.
    return m_nMapID;
}

void Area::SetMapType(const EMap::Type& _t) noexcept
{
}

const EMap::Type Area::GetMapType() const noexcept
{
    return EMap::Type();
}

bool Area::Create(const int& _mapID, const int& _w, const int& _h)
{
    if (nullptr != m_pField)
        return false;

    if (_w <= 0 || _h <= 0)
        return false;

    auto localMapInfo = MDBDataManager::GetInst().GetMapInfo(_mapID);
    if (nullptr == localMapInfo
        || localMapInfo->ActiveYN == 0)
        return false;


    m_nMapID = _mapID;
    m_eMapType = (EMap::Type)localMapInfo->GetMapType();

    //클라에서 어떻게 사용하게 될지 확인 필요
    Position localStartPos(_w * -0.5f, _h * -0.5f, 0.f);

    //맵 타입에 따라 별도로 설정할 영역
    switch (m_eMapType)
    {
    
    default:
    {
        m_nChannelCount = localMapInfo->Channel;
        m_nMaxChannelCount = localMapInfo->MaxChannelCount;

        if (m_nMaxChannelCount <= 0)
        {
            VIEW_WRITE_ERROR("MaxChannelCount is less 0 MapID:%d, MaxChannelCount: %d", m_nMapID, m_nMaxChannelCount);
            return false;
        }

        m_pField = new Field[m_nMaxChannelCount];
        for (size_t i = 0; i < m_nMaxChannelCount; i++)
        {
            //m_pField[i]
        }
    }
    break;
    }



    return true;
}
