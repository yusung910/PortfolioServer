#pragma once
#include <cstdint>

#if USE_SVALUE
enum class SValueType
{
    SV_BOOL = 1,
    SV_CHAR,
    SV_SHORT,
    SV_INT,
    SV_INT64,
    SV_FLOAT,
    SV_DOUBLE,
};

template <typename T, SValueType eType>
class SValue
{
public:
    T mValue;

    SValue()
    {
        mValue = 0;
    }
    ~SValue() = default;

    SValue(const SValue& nsv)
    {
        *this = nsv;
    }

    operator T()
    {
        return mValue;
    }

    operator T() const
    {
        return mValue;
    }

    bool operator==(T value)
    {
        return (mValue == value);
    }

    bool operator!=(T value)
    {
        return (mValue != value);
    }

    const SValue& operator=(const SValue& nsv)
    {
        *this = (T)nsv;
        return *this;
    }

    const SValue& operator=(T value)
    {
        mValue = value;
        return *this;
    }

    const SValue& operator+=(T value)
    {
        mValue += value;
        return *this;
    }

    const SValue& operator-=(T value)
    {
        mValue -= value;
        return *this;
    }
};

using SVbool = SValue<bool, SValueType::SV_BOOL>;
using SVchar = SValue<int8_t, SValueType::SV_CHAR>;
using SVshort = SValue<int16_t, SValueType::SV_SHORT>;
using SVint = SValue<int32_t, SValueType::SV_INT>;
using SVint64 = SValue<int64_t, SValueType::SV_INT64>;
using SVfloat = SValue<float, SValueType::SV_FLOAT>;
using SVdouble = SValue<double, SValueType::SV_DOUBLE>;

#else

using SVbool = bool;
using SVchar = int8_t;
using SVshort = int16_t;
using SVint = int32_t;
using SVint64 = int64_t;
using SVfloat = float;
using SVdouble = double;

#endif