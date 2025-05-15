#include "PGPPrivate.h"
#include "SYNCObjectDUserBuff.h"

SYNCObjectDUserBuff::SYNCObjectDUserBuff(ObjectTag)
{
}

SYNCObjectDUserBuff::~SYNCObjectDUserBuff()
{
}

void SYNCObjectDUserBuff::Set(const int& _skillModuleID, const int64_t& _remainMS, const int& _optionID)
{
    m_nSkillModuleID = _skillModuleID;
    m_nRemainMS = _remainMS;
    m_nOptionID = _optionID;
}

void SYNCObjectDUserBuff::Reset()
{
    m_nSkillModuleID = 0;
    m_nRemainMS = 0;
}
