#pragma once

#include "Position.h"
//X ���� �������� �ϴ� ���� ���� ���ϱ� ���� Ŭ����
//Z���� ����, 3D���� Yaw ���� ���Ѵ�
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

    //X,Y�� ����� ���õ� �Լ�?
    void SetNormalXY(float _inNormalX, float _inNormalY);
    void GetNormalXY(float _inNormalX, float _inNormalY) const;

    //���� Yaw ��ȯ
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
        // anticlockwise �ݽð����
        Direction lRslt = *this;
        lRslt.m_fYaw += _yaw;
        return lRslt;
    }

    inline Direction operator-(float _yaw) const
    {
        // anticlockwise �ݽð����
        Direction lRslt = *this;
        lRslt.m_fYaw -= _yaw;
        return lRslt;
    }
};

