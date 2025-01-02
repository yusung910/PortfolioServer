#include "stdafx.hxx"
#include "Fan.h"
#include "CmnMath.h"
#include <DirectXMath.h>
#include "Random.h"

Fan::Fan()
    : IShape(EShape::Fan)
{

}

Fan::~Fan()
{
    SafeDeleteArray(m_pVertexList);
}

bool Fan::IsValid() const
{
    if (m_fRadius <= 0.0f)
        return false;

    if (isnan(m_fRadius)
        || isinf(m_fRadius))
        return false;

    if (m_fAngleDegree <= 0.0f
        || m_fAngleDegree >= 360.0f
        || m_fDirAngleDegree < 0.0f
        || m_fDirAngleDegree >= 360.0f)
        return false;

    if (m_vDirection.GetLengthPow() <= 0.0f)
        return false;

    if (nullptr == m_pVertexList)
        return false;

    return true;
}

bool Fan::IsInArea(const float& _x, const float& _y) const
{
    if (nullptr == m_pVertexList)
        return false;

    Vec2 lCheckSP(_x, _y);
    Vec2 lCheckEP(99999.0f, _y);

    int lCrossCount = 0;

    for (int i = 0; i < m_nVertexCount; ++i)
    {
        if (true == IsCrossOver(lCheckSP, lCheckEP, m_pVertexList[i % m_nVertexCount], m_pVertexList[(i + 1) % m_nVertexCount]))
            lCrossCount++;

    }

    return (lCrossCount % 2) == 1;
}

bool Fan::GetRandomPosition(float& o_x, float& o_y) const
{
    if(false == IsValid())
        return false;
    float lStartAngle = m_fDirAngleDegree - (m_fAngleDegree * 0.5f);
    float lRandomAngle = Random::GetInst()->GetRandomRange(lStartAngle, lStartAngle + m_fAngleDegree);
    float lDist = Random::GetInst()->GetRandomRange(0.0f, m_fRadius);

    o_x = m_vCenter.x + cos(DegreeToRadian(lRandomAngle)) * lDist;
    o_y = m_vCenter.y + sin(DegreeToRadian(lRandomAngle)) * lDist;

    return IsInArea(o_x, o_y);
}

bool Fan::MakeData()
{
    if (m_fRadius <= 0.0f)
        return false;

    if (isnan(m_fRadius)
        || isinf(m_fRadius))
        return false;

    if (m_fAngleDegree <= 0.0f
        || m_fAngleDegree >= 360.0f
        || m_fDirAngleDegree < 0.0f
        || m_fDirAngleDegree >= 360.0f)
        return false;

    if (m_vDirection.GetLengthPow() <= 0.0f)
        return false;

    SafeDeleteArray(m_pVertexList);

    int lDivideCount = static_cast<int>(m_fAngleDegree / ARC_ROUGH_DIVIDE_DEGREE) + 1;

    m_nVertexCount = lDivideCount + 2;  //원점 + 시작 선 끝 점 + 나누는 수량
    m_pVertexList = new Vec2[m_nVertexCount];

    m_pVertexList[0].Set(m_vCenter.x, m_vCenter.y);

    float lDivideAngle = m_fAngleDegree / lDivideCount;

    float lStartAngle = m_fDirAngleDegree - (m_fAngleDegree * 0.5f);

    for (int i = 0; i <= lDivideCount; ++i)
    {
        m_pVertexList[1 + i] = m_vCenter;

        m_pVertexList[1 + i].x += cos(DegreeToRadian(lStartAngle + (i * lDivideAngle))) * m_fRadius;
        m_pVertexList[1 + i].y += sin(DegreeToRadian(lStartAngle + (i * lDivideAngle))) * m_fRadius;
    }

    return true;
}
