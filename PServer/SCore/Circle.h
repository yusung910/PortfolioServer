#pragma once
#include "SCoreAPI.h"
#include "IShape.h"

class SCoreAPI Circle : public IShape
{
private:
    float m_fRadius = 0.0f;

public:
    Circle();
    virtual ~Circle() = default;

    virtual bool IsValid() const;
    virtual bool IsInArea(const float& _x, const float& _y) const;
    virtual bool GetRandomPosition(float& o_x, float& o_y) const;

    bool SetRadius(const float& _rad);

};

