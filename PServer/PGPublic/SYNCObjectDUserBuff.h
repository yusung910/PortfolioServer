#pragma once

#include <PGObject.h>
class SYNCObjectDUserBuff : public PGObject<SYNCObjectDUserBuff>
{
public:
    
    int m_nSkillModuleID = 0;
    int m_nOptionID = 0;
    int64_t m_nRemainMS = 0;


    SYNCObjectDUserBuff(ObjectTag);
    ~SYNCObjectDUserBuff();

    void Set(const int& _skillModuleID, const int64_t& _remainMS, const int& _optionID);
    void Reset();
};

