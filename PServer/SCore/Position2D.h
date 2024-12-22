#pragma once
class Position2D
{
public:
    float x = 0.0f;
    float y = 0.0f;

public:
    Position2D(const float& _x = 0.0f, const float& _y = 0.0f);
    ~Position2D() = default;

    //연산자 오버라이딩 함수
    Position2D operator+(const Position2D& _val) const;
    Position2D operator-(const Position2D& _val) const;
    Position2D operator*(const float& _val) const;
    Position2D operator/(const float& _val) const;

    Position2D& operator=(const Position2D& _val);
    Position2D& operator+=(const Position2D& _val);
    Position2D& operator-=(const Position2D& _val);
    Position2D& operator*=(const float& _val);
    Position2D& operator/=(const float& _val);
    bool operator==(const Position2D& _val) const;
    bool operator!=(const Position2D& _val) const;


    //Set
    void Reset();
    void Set(const float& _x = 0.0f, const float& _y = 0.0f);
    void Set(Position2D& _pos);

    //정규화:
    //Vector의 크기가 1이고 방향 값만 있는 vector로 만든다
    bool Normalize();

    // Get
    float GetDistance(const Position2D& _val) const;
    float GetLength() const;
    float GetLengthPow() const;


    //By angle
    float GetDirectionDegree() const;
    void SetDirectionDegree(const float& _deg);
};

using Vec2 = Position2D;

