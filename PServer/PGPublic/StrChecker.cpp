#include "PGPPrivate.h"
#include "StrChecker.h"
#include <StringUtil.h>

#include "StrFilterKR.h"

StrChecker::StrChecker()
{

#ifdef KR_SERVICE
    m_umFilterList.insert_or_assign(EStrFilterType::Kor, new StrFilterKR);

#endif // KR_SERVICE
}

StrChecker::~StrChecker()
{
    m_pFilter = nullptr;
    for (auto lIt = m_umFilterList.begin(); lIt != m_umFilterList.end(); ++lIt)
    {
        SafeDelete(lIt->second);
    }

    m_umFilterList.clear();
}

bool StrChecker::ChangeFilterType(const EStrFilterType& _type)
{
    auto lIt = m_umFilterList.find(_type);
    if (lIt == m_umFilterList.end())
        return false;

    m_pFilter = lIt->second;
    return true;
}

bool StrChecker::IsValidStr(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (nullptr == m_pFilter)
        return false;

    if (false == m_pFilter->IsValidStr(_str, _min, _max))
        return false;

    return true;
}

bool StrChecker::IsValidStr(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStr(StringUtil::ToWideChar(_str), _min, _max);
}

bool StrChecker::IsValidStr8(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStr(StringUtil::UTF8_WSTR(_str), _min, _max);
}

bool StrChecker::IsValidStrWithSpecial(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (nullptr == m_pFilter)
        return false;

    if (false == m_pFilter->IsValidStrWithSpecial(_str, _min, _max))
        return false;

    return true;
}

bool StrChecker::IsValidStrWithSpecial(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStrWithSpecial(StringUtil::ToWideChar(_str), _min, _max);
}

bool StrChecker::IsValidStr8WithSpecial(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStrWithSpecial(StringUtil::UTF8_WSTR(_str), _min, _max);
}

bool StrChecker::IsValidStrOnlyASCII(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (nullptr == m_pFilter)
        return false;

    if (false == m_pFilter->IsValidASCII(_str, _min, _max))
        return false;

    return true;
}

bool StrChecker::IsValidStrOnlyASCII(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStrOnlyASCII(StringUtil::ToWideChar(_str), _min, _max);
}

bool StrChecker::IsValidStr8OnlyASCII(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStrOnlyASCII(StringUtil::UTF8_WSTR(_str), _min, _max);
}


bool StrChecker::IsValidStrAccountUKey(const std::wstring& _str, const size_t& _min, const size_t& _max)
{
    if (nullptr == m_pFilter)
        return false;

    if (false == m_pFilter->IsValidAccountUKey(_str, _min, _max))
        return false;

    return true;
}

bool StrChecker::IsValidStrAccountUKey(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStrAccountUKey(StringUtil::ToWideChar(_str), _min, _max);
}

bool StrChecker::IsValidStr8AccountUKey(const std::string& _str, const size_t& _min, const size_t& _max)
{
    return IsValidStrAccountUKey(StringUtil::UTF8_WSTR(_str), _min, _max);
}
