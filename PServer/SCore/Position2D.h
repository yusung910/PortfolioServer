#pragma once
class Position2D
{
public:
    float x = 0.0f;
    float y = 0.0f;

public:
    Position2D(const float& _x = 0.0f, const float& _y = 0.0f);
    ~Position2D() = default;

    //������ �������̵� �Լ�
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

    //����ȭ:
    //Vector�� ũ�Ⱑ 1�̰� ���� ���� �ִ� vector�� �����
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

