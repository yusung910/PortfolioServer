#pragma once
#include "SCoreAPI.h"
#include "IShape.h"

class Fan : public IShape
{
private:
    float m_fRadius = 0.0f;
    Vec2 m_vDirection = {};
    float m_nAngleDegree = 0.0f;

    //
    float m_fDirAngleDegree = 0.0f;
    Vec2* m_pVertexList = nullptr;
    int m_nVertexCount = 0;

public:
    Fan(); 
    virtual ~Fan();

    virtual bool IsValid() const;
    virtual bool IsInArea(const float& _x, const float& _y) const;
    virtual bool GetRandomPosition(float& o_x, float& o_y) const;

    virtual bool MakeData() override;
};

