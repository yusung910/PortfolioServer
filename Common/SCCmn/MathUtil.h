#pragma once

// 편의 매크로함수 추가
#define FD_PI 3.1415926535f
#define DegreeToRadian(x) (x / 180.0f * FD_PI)
#define RadianToDegree(x) (x * 180.0f / FD_PI)

namespace MathUtil
{
    // Reference from 
    //   math.isclose() at python.org
    //   std::numeric_limits::epsilon() at cppreference.com
    template<typename T>
    typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
        AlmostEqual(T x, T y,
            T relativeTolerance = std::numeric_limits<T>::epsilon(),
            T absoluteTolerance = std::numeric_limits<T>::epsilon())
    {
        return std::abs(x - y) <= std::max(
            relativeTolerance * std::max(std::abs(x), std::abs(y)),
            absoluteTolerance
        );
    }

    // Knuth's modulo from "modulo operation" at wikipedia
    template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
    T KnuthMod(T x, T d)
    {
        return x - d * std::floor(x / d);
    }
}