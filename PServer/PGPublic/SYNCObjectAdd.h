#pragma once

#include "SYNCObject.h"
#include "BaseObject.h"
#include "SYNCObjectDUserBuff.h"

class SYNCObjectAdd : public SYNCObject
{
private:
    float m_fPosX = 0.0f;
    float m_fPosY = 0.0f;
    int m_nDegree = 0;
    int m_nTemplateID = 0;

    std::string m_sString = {};

    //
    int64_t m_nPilgrimSeq = 0ll;
    int m_nHP = 0;

    int m_nServerID = 0;


public:
    SYNCObjectAdd();
    virtual ~SYNCObjectAdd();

    virtual void Reset();
    virtual void Set(BaseObject& _bo);

    virtual void Get(flatbuffers::FlatBufferBuilder& _fb, std::vector<flatbuffers::Offset<DSync>>& _list);

};

