#pragma once
#include "SCoreAPI.h"
#include "IShape.h"

class SCoreAPI Box : public IShape
{
private:
    float m_fWidth = 0.0f;
    float m_fHeight = 0.0f;
    float m_fRotationDegree = 0.0f;

    Position2D m_oRectPos[4];

public:
    Box();
    virtual ~Box() = default;

    virtual bool IsValid() const;
    virtual bool IsInArea(const float& _x, const float& _y) const;
    virtual bool GetRandomPosition(float& o_x, float& o_y) const;

    virtual bool MakeData() override;

    bool SetWidth(const float& _width);
    bool SetHeight(const float& _height);
    bool SetRotation(const float& _rot);
};

