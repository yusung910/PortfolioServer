#pragma once
#include "StrFilterInterface.h"

constexpr const wchar_t* REGEX_PATTERN_KR = L"([^A-Za-z0-9가-깋나-닣다-딯라-맇마-밓바-빟사-싷아-잏자-짛차-칳카-킿타-팋파-핗하-힣까-낗따-띻빠-삫싸-앃짜-찧])";
constexpr const wchar_t* REGEX_PATTERN_WITH_SPECIAL_KR = L"([^A-Za-z0-9ㄱ-힣!?,.()* ])";

class StrFilterKR : public StrFilterInterface
{
public:
    StrFilterKR();
    virtual ~StrFilterKR() = default;
    virtual bool IsFirstCharacter(const wchar_t& _ch);
};

