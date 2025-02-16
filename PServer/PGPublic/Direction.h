#pragma once

#include "Position.h"
//X 축을 기준으로 하는 각도 값을 구하기 위한 클래스
//Z축은 고정, 3D에서 Yaw 값을 구한다
class Direction
{
private:
    float m_fYaw;

public:
    Direction() : m_fYaw(0)
    {
    }

    void Reset();

    void SetYaw(float _yaw);
    void SetYaw360(int _yaw);
    void SetYaw3600(int _yaw);

    float GetYaw() const;
    int GetYaw360() const;
    int GetYaw3600() const;

    //X,Y의 방향과 관련된 함수?
    void SetNormalXY(float _inNormalX, float _inNormalY);
    void GetNormalXY(float _inNormalX, float _inNormalY) const;

    //직각 Yaw 반환
    Direction GetPerpendicular() const;
    int GetIncludedAngleDegree(const Direction& _dir) const;

public:
    inline Position operator*(float _scale) const
    {
        float lX = 0.0f, lY = 0.0f;
        GetNormalXY(lX, lY);
        return Position(lX * _scale, lY * _scale);
    }

    inline Direction operator+(float _yaw) const
    {
        // anticlockwise 반시계방향
        Direction lRslt = *this;
        lRslt.m_fYaw += _yaw;
        return lRslt;
    }

    inline Direction operator-(float _yaw) const
    {
        // anticlockwise 반시계방향
        Direction lRslt = *this;
        lRslt.m_fYaw -= _yaw;
        return lRslt;
    }
};

