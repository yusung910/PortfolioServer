#pragma once
#include "StrFilterInterface.h"

constexpr const wchar_t* REGEX_PATTERN_KR = L"([^A-Za-z0-9°¡-ƒ‰³ª-ˆ¢´Ù-‹L¶ó-¡¸¶-“J¹Ù-•½»ç-šï¾Æ-ŸçÀÚ-£ Â÷-¯†Ä«-µiÅ¸-»MÆÄ-À˜ÇÏ-ÆR±î-…ùµû-êºü-˜ª½Î-ÏÂ¥-Âö])";
constexpr const wchar_t* REGEX_PATTERN_WITH_SPECIAL_KR = L"([^A-Za-z0-9¤¡-ÆR!?,.()* ])";

class StrFilterKR : public StrFilterInterface
{
public:
    StrFilterKR();
    virtual ~StrFilterKR() = default;
    virtual bool IsFirstCharacter(const wchar_t& _ch);
};

