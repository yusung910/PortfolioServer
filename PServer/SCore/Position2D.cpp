#include "stdafx.hxx"
#include "Position2D.h"
#include <cmath>
#include "CmnMath.h"

Position2D::Position2D(const float& _x, const float& _y)
{
	Set(_x, _y);
}

Position2D Position2D::operator+(const Position2D& _val) const
{
	return Position2D(x + _val.x, y + _val.y);
}

Position2D Position2D::operator-(const Position2D& _val) const
{
	return Position2D(x - _val.x, y - _val.y);
}

Position2D Position2D::operator*(const float& _val) const
{
	return Position2D(x * _val, y * _val);
}

Position2D Position2D::operator/(const float& _val) const
{
	return Position2D(x / _val, y / _val);
}

Position2D& Position2D::operator=(const Position2D& _val)
{
	Set(_val.x, _val.y);
	return *this;
}

Position2D& Position2D::operator+=(const Position2D& _val)
{
	x += _val.x;
	y += _val.y;
	return *this;
}

Position2D& Position2D::operator-=(const Position2D& _val)
{
	x -= _val.x;
	y -= _val.y;
	return *this;
}

Position2D& Position2D::operator*=(const float& _val)
{
	x *= _val;
	y *= _val;
	return *this;
}

Position2D& Position2D::operator/=(const float& _val)
{
	if (_val != 0.0f)
	{
		x /= _val;
		y /= _val;
	}
	return *this;
}

bool Position2D::operator==(const Position2D& _val) const
{
	if (x == _val.x
		&& y == _val.y)
		return true;
	return false;
}

bool Position2D::operator!=(const Position2D& _val) const
{
	if (x == _val.x
		|| y == _val.y)
		return false;
	return true;
}

void Position2D::Reset()
{
	Set();
}

void Position2D::Set(const float& _x, const float& _y)
{
	x = _x;
	y = _y;
}

void Position2D::Set(Position2D& _pos)
{
	x = _pos.x;
	y = _pos.y;
}

bool Position2D::Normalize()
{
	float fLength = GetLength();
	if (0.0f == fLength)
		return false;

	x /= fLength;
	y /= fLength;

	return true;
}

float Position2D::GetDistance(const Position2D& _val) const
{
	return Position2D(_val.x - x, _val.y - y).GetLength();
}

float Position2D::GetLength() const
{
	return sqrtf(GetLengthPow());
}

float Position2D::GetLengthPow() const
{
	return x * x + y * y;
}

/*
   ↑ 방향이 0도, 시계 방향으로 각도 증가.
*/
float Position2D::GetDirectionDegree() const
{
	Position2D lRetVal(*this);
	if (false == lRetVal.Normalize())
		return 0.0f;

	return DEGREE_180 * atan2f(y, x) / MATH_PI;
}

void Position2D::SetDirectionDegree(const float& _deg)
{
	if (true == IsValidValue(_deg))
		return;

	float lDegree = DegreeToRadian(_deg);

	x = cos(lDegree);
	y = sin(lDegree);
}
