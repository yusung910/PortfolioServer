#include "stdafx.hxx"
#include "Box.h"
#include "CmnMath.h"
#include <DirectXMath.h>
#include "Random.h"

Box::Box()
    :IShape(EShape::Box)
{

}

bool Box::IsValid() const
{
    //isnan() : float 값이 nan(n/0)인지 검사
    //isinf() : float 값이 무한 대역인지 확인
    if (true == isnan(m_fWidth)
        || true == isinf(m_fWidth)
        || true == isnan(m_fHeight)
        || true == isinf(m_fHeight)
        || true == isnan(m_fRotationDegree)
        || true == isinf(m_fRotationDegree)
        || m_fWidth <= 0.0f
        || m_fHeight <= 0.0f
        || m_fRotationDegree < 0.0f
        || m_fRotationDegree >= DEGREE_360)
        return false;


    return true;
}

bool Box::IsInArea(const float& _x, const float& _y) const
{
    Vec2 lCheckSP(_x, _y);
    Vec2 lCheckEP(99999.0f, _y);

    int lCrossCount = 0;

    for (int i = 0; i < 4; i++)
    {
        if (true == IsCrossOver(lCheckSP, lCheckEP
            , m_oRectPos[i % 4], m_oRectPos[(i + 1) % 4]))
            lCrossCount++;
    }
    return (lCrossCount % 2) == 1;
}

bool Box::GetRandomPosition(float& o_x, float& o_y) const
{
    if (false == IsValid())
        return false;

    // -1.0 ~ 1.0 ���� ����
    float lx = Random::GetInst()->GetRandomRange(0, 200) * 0.01f;
    float ly = Random::GetInst()->GetRandomRange(0, 200) * 0.01f;
    lx -= 1.0f;
    ly -= 1.0f;

    lx *= m_fWidth;
    ly *= m_fHeight;

    DirectX::XMVECTOR lCalcPos;
    lCalcPos.m128_f32[0] = lx;
    lCalcPos.m128_f32[1] = ly;
    lCalcPos.m128_f32[2] = 0.0f;
    lCalcPos.m128_f32[3] = 0.0f;

    //https://learn.microsoft.com/ko-kr/windows/win32/api/directxmath/nf-directxmath-xmmatrixrotationz
    //XMMatrixRotationZ(): Z축으로 회전하는 행렬을 빌드
    DirectX::XMMATRIX lMat = DirectX::XMMatrixRotationZ(m_fRotationDegree);

    //행렬을 2D Vector로 변환
    lCalcPos = DirectX::XMVector2Transform(lCalcPos, lMat);

    o_x = lCalcPos.m128_f32[0] + m_vCenter.x;
    o_y = lCalcPos.m128_f32[0] + m_vCenter.y;

    return true;
}

bool Box::MakeData()
{
    if (false == IsValid())
        return false;

    DirectX::XMFLOAT2 lBoxPos[4];

    lBoxPos[0].x = -m_fWidth;
    lBoxPos[0].y = m_fHeight;

    lBoxPos[1].x = m_fWidth;
    lBoxPos[1].y = m_fHeight;

    lBoxPos[2].x = m_fWidth;
    lBoxPos[2].y = -m_fHeight;

    lBoxPos[3].x = -m_fWidth;
    lBoxPos[3].y = -m_fHeight;

    DirectX::XMMATRIX lMat = DirectX::XMMatrixRotationZ(DegreeToRadian(m_fRotationDegree));

    for (int i = 0; i < 4; i++)
    {
        auto lvPT = DirectX::XMLoadFloat2(&lBoxPos[i]);
        lvPT = DirectX::XMVector2Transform(lvPT, lMat);
        DirectX::XMStoreFloat2(&lBoxPos[i], lvPT);

        m_oRectPos[i].x = lBoxPos[i].x + m_vCenter.x;
        m_oRectPos[i].y = lBoxPos[i].y + m_vCenter.y;
    }

    return true;
}

bool Box::SetWidth(const float& _width)
{
    if (_width < 0.0f)
        return false;

    m_fWidth = _width;
    return true;
}

bool Box::SetHeight(const float& _height)
{
    if (_height < 0.0f)
        return false;

    m_fHeight = _height;
    return true;
}

bool Box::SetRotation(const float& _rot)
{
    m_fRotationDegree = DegreeNormalize(_rot);
    return false;
}


