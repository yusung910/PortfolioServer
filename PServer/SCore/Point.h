#pragma once
#include "SCoreAPI.h"
#include "IShape.h"

class Point : public IShape
{
public:
    Point();
    virtual ~Point() = default;

    virtual bool IsValid() const;
    virtual bool IsInArea(const float& _x, const float& _y) const;
    virtual bool GetRandomPosition(float& o_x, float& o_y) const;
};

