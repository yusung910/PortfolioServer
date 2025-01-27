#include "PGPPrivate.h"
#include "StrFilterInterface.h"
#include "CheckValueRange.h"

StrFilterInterface::StrFilterInterface(const EStrFilterType& _type)
{
    m_eFilterType = _type;
}

bool StrFilterInterface::IsValidASCII(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (false == IsBetween(_min, _max, _str.size()))
        return false;

    return !_IsFiltered(_str, m_oAsciiPattern);
}

bool StrFilterInterface::IsValidAccountToken(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (false == IsBetween(_min, _max, _str.size()))
        return false;

    return !_IsFiltered(_str, m_oAccountTokenPattern);
}

bool StrFilterInterface::HasInjection(const std::wstring& _str)
{
    return _IsFiltered(_str, m_oInjectionPattern);
}

bool StrFilterInterface::IsValidStr(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (false == IsBetween(_min, _max, _str.size()))
        return false;

    if (true == _str.empty())
        return false;

    if (false == IsFirstCharacter(_str.at(0)))
        return false;

    return !_IsFiltered(_str, m_oFilter);
}

bool StrFilterInterface::IsValidStrWithSpecial(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (false == IsBetween(_min, _max, _str.size()))
        return false;

    if (true == _str.empty())
        return false;

    if (false == IsFirstCharacter(_str.at(0)))
        return false;

    return !_IsFiltered(_str, m_oFilterSpecial);
}

const EStrFilterType& StrFilterInterface::GetFilterType() const noexcept
{
    return m_eFilterType;
}

bool StrFilterInterface::_IsKorCharacter(const wchar_t& _ch)
{
    if (_ch >= L'°¡' && _ch <= L'ÆR')
        return true;
    return false;
}

bool StrFilterInterface::_IsEngCharacter(const wchar_t& _ch)
{
    if (_ch >= L'A' && _ch <= L'Z')
        return true;
    if (_ch >= L'a' && _ch <= L'z')
        return true;

    return false;
}

bool StrFilterInterface::_IsNumberCharacter(const wchar_t& _ch)
{
    if (_ch >= L'0' && _ch <= L'9')
        return true;
    return false;
}

bool StrFilterInterface::_IsFiltered(const std::wstring& _str, std::wregex& _reg)
{
    std::wsmatch lMatch;
    return std::regex_search(_str, lMatch, _reg);
}
