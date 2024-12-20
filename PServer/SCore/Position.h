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

    //연산자 오버라이딩 함수
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

    //정규화:
    //Vector의 크기가 1이고 방향 값만 있는 vector로 만든다
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