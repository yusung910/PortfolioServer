#pragma once
#include <RefSingleton.h>
#include "StrFilterInterface.h"
#include <unordered_map>
class StrChecker : public RefSingleton<StrChecker>
{
private:
    StrFilterInterface* m_pFilter = nullptr;
    std::unordered_map<EStrFilterType, StrFilterInterface*> m_umFilterList;
public:
    StrChecker();
    virtual ~StrChecker();
    
    bool ChangeFilterType(const EStrFilterType& _type);

public:
    bool IsValidStr(const std::wstring& _str, const size_t& _min, const size_t& _max);
    bool IsValidStr(const std::string& _str, const size_t& _min, const size_t& _max);
    bool IsValidStr8(const std::string& _str, const size_t& _min, const size_t& _max);

    bool IsValidStrWithSpecial(const std::wstring& _str, const size_t& _min, const size_t& _max);
    bool IsValidStrWithSpecial(const std::string& _str, const size_t& _min, const size_t& _max);
    bool IsValidStr8WithSpecial(const std::string& _str, const size_t& _min, const size_t& _max);

    bool IsValidStrOnlyASCII(const std::wstring& _str, const size_t& _min, const size_t& _max);
    bool IsValidStrOnlyASCII(const std::string& _str, const size_t& _min, const size_t& _max);
    bool IsValidStr8OnlyASCII(const std::string& _str, const size_t& _min, const size_t& _max);

    bool IsValidStrAccountToken(const std::wstring& _str, const size_t& _min, const size_t& _max);
    bool IsValidStrAccountToken(const std::string& _str, const size_t& _min, const size_t& _max);
    bool IsValidStr8AccountToken(const std::string& _str, const size_t& _min, const size_t& _max);
};

