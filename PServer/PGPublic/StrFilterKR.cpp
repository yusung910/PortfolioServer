#include "PGPPrivate.h"
#include "StrFilterKR.h"

StrFilterKR::StrFilterKR()
    :StrFilterInterface(EStrFilterType::Kor)
{
    m_oFilter = std::wregex(REGEX_PATTERN_KR);
    m_oFilterSpecial = std::wregex(REGEX_PATTERN_WITH_SPECIAL_KR);
}

bool StrFilterKR::IsFirstCharacter(const wchar_t& _ch)
{
    if (false == _IsKorCharacter(_ch)
        && false == _IsEngCharacter(_ch))
        return false;

    return true;
}
