#include "PGPPrivate.h"
#include "Direction.h"
#include "Position.h"
#include "MathUtil.h"

#ifndef PI
#define PI 3.14159265358979323846
#endif

void Direction::Reset()
{
    m_fYaw = 0.0f;
}

void Direction::SetYaw(float _yaw)
{
    m_fYaw = _yaw;
}

void Direction::SetYaw360(int _yaw)
{
    m_fYaw = (float)_yaw;
}

void Direction::SetYaw3600(int _yaw)
{
    m_fYaw = _yaw / 10.0f;
}

float Direction::GetYaw() const
{
    return m_fYaw;
}

int Direction::GetYaw360() const
{
    return (int)round(m_fYaw + 0.5f);
}

int Direction::GetYaw3600() const
{
    return (int)round((m_fYaw * 10.0f) + 0.5f);
}

void Direction::SetNormalXY(float _inNormalX, float _inNormalY)
{
    //http://choryeonworkshop.tistory.com/124
    m_fYaw = 180.f * atan2(_inNormalY, _inNormalX) / (float)PI;
}

void Direction::GetNormalXY(float _inNormalX, float _inNormalY) const
{
    _inNormalX = cos(m_fYaw / 180.0f * (float)PI);
    _inNormalY = sin(m_fYaw / 180.0f * (float)PI);
}

Direction Direction::GetPerpendicular() const
{
    Direction lPerp;
    lPerp.SetYaw(m_fYaw + 90.0f);
    return lPerp;
}

int Direction::GetIncludedAngleDegree(const Direction& _dir) const
{
    return static_cast<int>(
        roundf(
            std::min(
                MathUtil::KnuthMod(m_fYaw - _dir.m_fYaw, 360.0f)
                , MathUtil::KnuthMod(_dir.m_fYaw - m_fYaw, 360.0f)
            )
        )
    );
}
