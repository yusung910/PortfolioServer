#pragma once
#include <cmath>

constexpr float MATH_PI = 3.14159265f;

constexpr float DEGREE_180 = 180.0f;
constexpr float DEGREE_360 = 360.0f;
constexpr float RADIAN_180 = MATH_PI;

//호도법 -> 원에서 반지름 길이 만큼의 각
constexpr float RADIAN_360 = 2.0f * MATH_PI;

constexpr float DEGREE_TO_RADIAN = MATH_PI / 180.0f;
constexpr float RADIAN_TO_DEGREE = 180.0f / MATH_PI;

inline bool IsValidValue(const float& _val)
{
    if (true == isnan(_val) || true == isinf(_val))
        return false;

    return true;
}


// Math Angle function
//Radian 정규화 함수
inline float RadianNormalize(const float& _rad)
{
    if (false == IsValidValue(_rad))
        return 0.0f;

    //부동 소수점 나머지 연산(a/b)
    float lRet = fmodf(_rad, RADIAN_360);
    
    if (lRet < 0.0f)
        lRet += RADIAN_360;

    return lRet;
}

inline float DegreeNormalize(const float& _deg)
{
    if (false == IsValidValue(_deg))
        return 0.0f;

    //부동 소수점 나머지 연산(a/b)
    float lRet = fmodf(_deg, DEGREE_360);

    if (lRet < 0.0f)
        lRet += DEGREE_360;

    return lRet;
}

inline float DegreeToRadian(const float& _deg)
{
    return DegreeNormalize(_deg) * DEGREE_TO_RADIAN;
}

inline float RadianToDegree(const float& _deg)
{
    return RadianNormalize(_deg) * RADIAN_TO_DEGREE;
}