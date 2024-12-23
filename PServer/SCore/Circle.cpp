#include "stdafx.hxx"
#include "Circle.h"
#include "Random.h"
#include "CmnMath.h"

Circle::Circle()
    :IShape(EShape::Circle)
{
}

bool Circle::IsValid() const
{
    if (m_fRadius <= 0.0f)
        return false;
    return true;
}

bool Circle::IsInArea(const float& _x, const float& _y) const
{
    if (false == IsValid())
        return false;

    return m_vCenter.GetDistance(Vec2(_x, _y));
}

bool Circle::GetRandomPosition(float& o_x, float& o_y) const
{
    if (false == IsValid())
        return false;

    float lRad = Random::GetInst()->GetRandomRange(0.0f, RADIAN_360);
    float lDist = Random::GetInst()->GetRandomRange(0.0f, m_fRadius);

    o_x = m_vCenter.x + (cosf(m_fRadius) * lDist);
    o_y = m_vCenter.y + (sinf(m_fRadius) * lDist);

    return true;
}

bool Circle::SetRadius(const float& _rad)
{
    if (_rad <= 0.0f)
        return false;

    m_fRadius = _rad;
    return true;
}
