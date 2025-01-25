#include "stdafx.hxx"
#include "Position.h"
#include <cmath>
#include "CmnMath.h"

Position::Position(const float& _x, const float& _y, const float& _z)
{
    Set(_x, _y, _z);
}

Position Position::operator+(const Position& _val) const
{
    return Position(x + _val.x, y + _val.y, z + _val.z);
}

Position Position::operator-(const Position& _val) const
{
    return Position(x - _val.x, y - _val.y, z - _val.z);
}

Position Position::operator*(const Position& _val) const
{
    return Position(x * _val.x, y * _val.y, z * _val.z);
}

Position Position::operator/(const Position& _val) const
{
    return Position(x / _val.x, y / _val.y, z / _val.z);
}

Position& Position::operator=(const Position& _val)
{
    Set(_val.x, _val.y, _val.z);
    return *this;
}

Position& Position::operator+=(const Position& _val)
{
    x += _val.x;
    y += _val.y;
    z += _val.z;
    return *this;
}

Position& Position::operator-=(const Position& _val)
{
    x -= _val.x;
    y -= _val.y;
    z -= _val.z;
    return *this;
}

Position& Position::operator*=(const float& _val)
{
    x *= _val;
    y *= _val;
    z *= _val;
    return *this;
}

Position& Position::operator/=(const float& _val)
{
    if (_val != 0.0f)
    {
        float ldiv = 1.0f / _val;
        x *= ldiv;
        y *= ldiv;
        z *= ldiv;
    }

    return *this;
}

bool Position::operator==(const Position& _val) const
{
    if (x == _val.x
        && y == _val.y
        && z == _val.z)
        return true;
    return false;
}

bool Position::operator!=(const Position& _val) const
{
    if (x == _val.x
        && y == _val.y
        && z == _val.z)
        return false;
    return true;
}

void Position::Reset()
{
    Set(0, 0, 0);
}

void Position::Set(const float& _x, const float& _y, const float& _z)
{
    x = _x;
    y = _y;
    z = _z;
}

void Position::Set(Position& _pos)
{
    x = _pos.x;
    y = _pos.y;
    z = _pos.z;
}

bool Position::Normalize()
{
    float lLength = GetLength();
    if (lLength == 0.0f)
        return false;
    *this /= lLength;
    return true;
}

float Position::GetDistance(const Position& _other) const
{
    return Position(_other.x - x, _other.y - y).GetLength();
}

float Position::GetDistance3D(const Position& _other) const
{
    return Position(_other.x - x, _other.y - y, _other.z - z).GetLength();
}

float Position::GetXYDistanceNotSqrt(const Position& _other) const
{
    float lDx = abs(_other.x - this->x);
    float lDy = abs(_other.y - this->y);

    return (lDx > lDy) ? lDx : lDy;
}

float Position::GetLength() const
{
    //sqrtf Á¦°ö±Ù
    return sqrtf(GetLengthPow());
}

float Position::GetLengthPow() const
{
    return x * x + y * y + z * z;
}

float Position::GetDirectionDegree2D() const
{
    Position lRet(*this);
    lRet.z = 0.f;

    if (false == lRet.Normalize())
        return 0.f;

    //atan2f() -> y / x 
    return DEGREE_180 * atan2f(y, x) / MATH_PI;
}

void Position::SetDirectionDegree2D(const float& _deg)
{
    if (true == IsValidValue(_deg))
        return;
    float lDegree = DegreeToRadian(_deg);

    x = cos(lDegree);
    y = sin(lDegree);
    z = 0.0f;

}
