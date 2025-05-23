#pragma once

template <typename T>
static bool IsBetween(const T& _begin, const T& _end, const T& _val)
{
    if (_begin < _end)
    {
        if (_begin > _val
            || _end < _val)
            return false;
    }
    else
    {
        if (_begin < _val
            || _end > _val)
            return false;
    }

    return true;
}


template <typename T>
static void SetInRange(const T& _v1, const T& _v2, T& _val)
{
    if (_v1 < _v2)
    {
        _val = (_v1 > _val) ? _v1 : _val;
        _val = (_v2 < _val) ? _v2 : _val;
    }
    else
    {
        _val = (_v2 > _val) ? _v2 : _val;
        _val = (_v1 < _val) ? _v1 : _val;
    }
}