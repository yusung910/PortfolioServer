#include "stdafx.hxx"
#include "Point.h"
#include "CmnMath.h"
#include <DirectXMath.h>
#include "Random.h"

Point::Point()
    :IShape(EShape::Point)
{
}

bool Point::IsValid() const
{
    return true;
}

bool Point::IsInArea(const float& _x, const float& _y) const
{
    return m_vCenter.x == _x && m_vCenter.y == _y;
}

bool Point::GetRandomPosition(float& o_x, float& o_y) const
{
    o_x = m_vCenter.x;
    o_y = m_vCenter.y;
    return true;
}
