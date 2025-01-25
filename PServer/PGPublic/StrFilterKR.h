#pragma once
#include "StrFilterInterface.h"

constexpr const wchar_t* REGEX_PATTERN_KR = L"([^A-Za-z0-9��-����-����-�L��-����-�J��-����-���-����-����-��ī-�iŸ-�M��-����-�R��-����-���-����-��¥-��])";
constexpr const wchar_t* REGEX_PATTERN_WITH_SPECIAL_KR = L"([^A-Za-z0-9��-�R!?,.()* ])";

class StrFilterKR : public StrFilterInterface
{
public:
    StrFilterKR();
    virtual ~StrFilterKR() = default;
    virtual bool IsFirstCharacter(const wchar_t& _ch);
};

