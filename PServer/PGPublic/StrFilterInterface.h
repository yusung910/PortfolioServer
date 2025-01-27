#pragma once
#include <string>
#include <regex>

enum class EStrFilterType
{
    None = 0,
    Kor,
    Eng,

    Max
};

// common Regex PATTERN
// non Latin 문자열 추출
constexpr const wchar_t* REGEX_PATTERN_ASCII = L"([^A-Za-z0-9])";
//
constexpr const wchar_t* REGEX_PATTERN_ASCII_ACCOUNT_UKEY = L"([^A-Za-z0-9_-])";
//
constexpr const wchar_t* REGEX_PATTERN_INJECTION = L"(['%+*@])";

class StrFilterInterface
{
protected:
    std::wregex m_oFilter = {};
    std::wregex m_oFilterSpecial = {};

private:
    EStrFilterType m_eFilterType = EStrFilterType::None;

    std::wregex m_oAsciiPattern = std::wregex(REGEX_PATTERN_ASCII);
    std::wregex m_oAccountTokenPattern = std::wregex(REGEX_PATTERN_ASCII_ACCOUNT_UKEY);
    std::wregex m_oInjectionPattern = std::wregex(REGEX_PATTERN_INJECTION);

public:
    StrFilterInterface(const EStrFilterType& _type);
    virtual ~StrFilterInterface() = default;

    virtual bool IsFirstCharacter(const wchar_t& _ch) = 0;

    bool IsValidASCII(const std::wstring& _str, const size_t& _min, const size_t& _max);
    bool IsValidAccountToken(const std::wstring& _str, const size_t& _min, const size_t& _max);

    bool HasInjection(const std::wstring& _str);

    bool IsValidStr(const std::wstring& _str, const size_t& _min, const size_t& _max);
    bool IsValidStrWithSpecial(const std::wstring& _str, const size_t& _min, const size_t& _max);

    const EStrFilterType& GetFilterType() const noexcept;
protected:
    bool _IsKorCharacter(const wchar_t& _ch);
    bool _IsEngCharacter(const wchar_t& _ch);
    bool _IsNumberCharacter(const wchar_t& _ch);

private:
    bool _IsFiltered(const std::wstring& _str, std::wregex& _reg);
};