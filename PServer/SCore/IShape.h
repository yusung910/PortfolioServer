#pragma once
#include "SCoreAPI.h"
#include "Position.h"
#include "Position2D.h"

enum class EShape
{
	None = 0,

	Point,

	Box,
	Circle,
	Fan,	// 부채꼴

	Max,

	Polygon,
	Donut
};

/*!
 *  게임에서 사용하게 될 도형의 parent 클래스
 *  각각의 도형을 회전시키거나 이동시킬 때는 DirectX 함수를 이용한다
 */
class SCoreAPI IShape
{
protected:
	EShape m_eShapeType = EShape::None;
	Vec2 m_vCenter = { 0.0f, 0.0f };

public:
	IShape(const EShape& _type)
	{
		m_eShapeType = _type;
	}

	virtual ~IShape() = default;

	virtual bool IsValid() const = 0;
	virtual bool IsInArea(const float& _x, const float& _y) const = 0;
	virtual bool GetRandomPosition(float& o_x, float& o_y) const = 0;

	//
	virtual bool MakeData()
	{
		return true;
	}

	void SetPosition(const Position& _pos)
	{
		m_vCenter.Set(_pos.x, _pos.y);
	}

	const EShape& GetShapeType() const
	{
		return m_eShapeType;
	}

	bool IsInArea(const Position& _pos) const
	{
		return IsInArea(_pos.x, _pos.y);
	}

	bool GetRandomPosition(Position& o_pos) const
	{
		o_pos.z = 0.0f;
		return GetRandomPosition(o_pos.x, o_pos.y);
	}

protected:
	// ���ڰ����� ���޹��� �� ����(vector A, vector B)�� ��ġ���� �˻��Ѵ�
	// ���� ��ǥ���� ���� ���� �̿��ؼ� ���ϴ� ����� ���� �ٸ�.
	bool IsCrossOver(const Vec2& _beginPt1
				   , const Vec2& _endPt1
				   , const Vec2& _beginPt2
		           , const Vec2& _endPt2 ) const
	{
		float t, s;

		//https://bowbowbow.tistory.com/14
		float lUnder = (_endPt2.y - _beginPt2.y) * (_endPt1.x - _beginPt1.x) - (_endPt2.x - _beginPt2.x) * (_endPt1.y - _beginPt1.y);

		//���� ���� ����
		if (lUnder == 0.0f)
			return false;
		
		float _t = (_endPt2.x - _beginPt2.x) * (_beginPt1.y - _beginPt2.y) - (_endPt2.y - _beginPt2.y) * (_beginPt1.x - _beginPt2.x);
		float _s = (_endPt1.x - _beginPt1.x) * (_beginPt1.y - _beginPt2.y) - (_endPt1.y - _beginPt1.y) * (_beginPt1.x - _beginPt2.x);

		t = _t / lUnder;
		s = _s / lUnder;

		if (t < 0.0 || t>1.0 || s < 0.0 || s>1.0)
			return false;

		if (_t == 0 && _s == 0)
			return false;

		return true;
	}

};

