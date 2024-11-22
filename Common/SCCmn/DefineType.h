#pragma once

template <typename T>
class StaticPolymorphism
{
public:
    T& Underlying() { return static_cast<T&>(*this); }
    const T& Underlying() const { return static_cast<const T&>(*this); }
};

template <typename T>
class ArithmeticOperators : public StaticPolymorphism<T>
{
public: // assignment
    T& operator+=(const T& other)
    {
        this->Underlying().GetValue() += other.GetValue();
        return this->Underlying();
    }

    T& operator-=(const T& other)
    {
        this->Underlying().GetValue() -= other.GetValue();
        return this->Underlying();
    }

    T& operator*=(const T& other)
    {
        this->Underlying().GetValue() *= other.GetValue();
        return this->Underlying();
    }

    T& operator/=(const T& other)
    {
        this->Underlying().GetValue() /= other.GetValue();
        return this->Underlying();
    }

    T& operator%=(const T& other)
    {
        this->Underlying().GetValue() %= other.GetValue();
        return this->Underlying();
    }

public: // Increment/decrement
    T& operator++()
    {
        this->Underlying().GetValue()++;
        return this->Underlying();
    }

    T& operator--()
    {
        this->Underlying().GetValue()--;
        return this->Underlying();
    }

    T operator++(int)
    {
        T temp = this->Underlying();
        this->Underlying().GetValue()++;
        return temp;
    }

    T operator--(int)
    {
        T temp = this->Underlying();
        this->Underlying().GetValue()--;
        return temp;
    }

public: // arithmetic
    T operator+() const
    {
        return T(+(this->Underlying().GetValue()));
    }

    T operator-() const
    {
        return T(-(this->Underlying().GetValue()));
    }

    T operator+(const T& other) const
    {
        return static_cast<T>(this->Underlying().GetValue() + other.GetValue());
    }

    T operator-(const T& other) const
    {
        return static_cast<T>(this->Underlying().GetValue() - other.GetValue());
    }

    T operator*(const T& other) const
    {
        return static_cast<T>(this->Underlying().GetValue() * other.GetValue());
    }

    T operator/(const T& other) const
    {
        return static_cast<T>(this->Underlying().GetValue() / other.GetValue());
    }

    T operator%(const T& other) const
    {
        return static_cast<T>(this->Underlying().GetValue() % other.GetValue());
    }

    friend std::ostream& operator<<(std::ostream& os, const ArithmeticOperators<T>& t)
    {
        return os << t.Underlying().GetValue();
    }

public: // comparison
    bool operator==(const T& other) const
    {
        return this->Underlying().GetValue() == other.GetValue();
    }

    bool operator!=(const T& other) const
    {
        return this->Underlying().GetValue() != other.GetValue();
    }

    bool operator<(const T& other) const
    {
        return this->Underlying().GetValue() < other.GetValue();
    }

    bool operator>(const T& other) const
    {
        return this->Underlying().GetValue() > other.GetValue();
    }

    bool operator<=(const T& other) const
    {
        return this->Underlying().GetValue() <= other.GetValue();
    }

    bool operator>=(const T& other) const
    {
        return this->Underlying().GetValue() >= other.GetValue();
    }
};

template <typename T, typename PhantomType>
class StrongTypedef : public ArithmeticOperators<StrongTypedef<T, PhantomType>>
{
private:
    T m_value;

public:
    StrongTypedef() : m_value{} {}
    explicit StrongTypedef(const T& value) : m_value(value) {}

    T& GetValue() { return m_value; }
    const T& GetValue() const { return m_value; }

    explicit operator T() const { return m_value; }
};

namespace std
{
    template <typename T, typename P>
    struct hash<StrongTypedef<T, P>>
    {
        size_t operator()(const StrongTypedef<T, P>& t) const
        {
            return hash<T>()(t.GetValue());
        }
    };
}

#define STRONG_TYPEDEF(primitiveType, name)\
	using name = StrongTypedef<primitiveType, struct name##_tag>

STRONG_TYPEDEF(int, HostID);