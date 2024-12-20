#pragma once
#include "SCoreAPI.h"

class SCoreAPI Position
{
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

public:
    Position(const float& _x = 0.0f, const float& _y = 0.0f, const float& _z = 0.0f);
    ~Position() = default;

    //������ �������̵� �Լ�
    Position operator+(const Position& _val) const;
    Position operator-(const Position& _val) const;
    Position operator*(const Position& _val) const;
    Position operator/(const Position& _val) const;

    Position& operator=(const Position& _val);
    Position& operator+=(const Position& _val);
    Position& operator-=(const Position& _val);
    Position& operator*=(const float& _val);
    Position& operator/=(const float& _val);
    bool operator==(const Position& _val) const;
    bool operator!=(const Position& _val) const;


    //Set
    void Reset();
    void Set(const float& _x = 0.0f, const float& _y = 0.0f, const float& _z = 0.0f);

    //����ȭ:
    //Vector�� ũ�Ⱑ 1�̰� ���� ���� �ִ� vector�� �����
    bool Normalize();

    // Get
    float GetDistance(const Position& _other) const;
    float GetDistance3D(const Position& _other) const;
    float GetXYDistanceNotSqrt(const Position& _other) const;

    float GetLength() const;
    float GetLengthPow() const;


    //By angle
    float GetDirectionDegree2D() const;
    void SetDirectionDegree2D(const float& _deg);

};

using Vec3 = Position;