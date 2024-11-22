#pragma once

#if USE_SVALUE
typedef enum SValueType {
    SV_BOOL = 1,
    SV_CHAR,
    SV_SHORT,
    SV_INT,
    SV_INT64,
    SV_FLOAT,
    SV_DOUBLE,
}SValueType;

template <typename T, SValueType eType>
class SValue
{
public:
    T m_sValue;

    SValue()
    {
        m_sValue = 0;
    }
    ~SValue()
    {

    }
    SValue(const SValue& nsv)
    {
        *this = nsv;
    }
    operator T()
    {
        return m_sValue;
    }
    operator T() const
    {
        return m_sValue;
    }
    bool operator==(T value)
    {
        return (m_sValue == value);
    }
    bool operator!=(T value)
    {
        return (m_sValue != value);
    }
    const SValue& operator=(const SValue& nsv)
    {
        *this = (T)nsv;
        return *this;
    }
    const SValue& operator=(T value)
    {
        m_sValue = value;
        return *this;
    }
    const SValue& operator+=(T value)
    {
        m_sValue += value;
        return *this;
    }
    const SValue& operator-=(T value)
    {
        m_sValue -= value;
        return *this;
    }
};
typedef SValue<bool, SV_BOOL>		SVbool;
typedef SValue<int8_t, SV_CHAR>		SVchar;
typedef SValue<int16_t, SV_SHORT>	SVshort;
typedef SValue<int32_t, SV_INT>		SVint;
typedef SValue<int64_t, SV_INT64>	SVint64;
typedef SValue<float, SV_FLOAT>		SVfloat;
typedef SValue<double, SV_DOUBLE>	SVdouble;
typedef std::string					SVString;
#else

typedef bool						SVbool;
typedef int8_t						SVchar;
typedef int16_t						SVshort;
typedef int32_t						SVint;
typedef int64_t						SVint64;
typedef float						SVfloat;
typedef double						SVdouble;

typedef std::string					SVString;

#endif