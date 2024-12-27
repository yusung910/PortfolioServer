#pragma once
#include "SecureValueClass.h"
#include "GlobalEnum.h"
#include "GlobalConst.h"


#ifdef SERVER_BUILD
#	pragma warning(push)
#	pragma warning(disable:4819)
#else   
#define CLIENT_ONLY

#endif // SERVER_BUILD

//기본적인것만 세팅, 추후 추가되는 컨텐츠에 따른 작업 필요
struct MDBHeroStat;

struct MDBHeroStat
{
    SVint Seq;

    typedef union
    {
        struct
        {
            SVint None;

            SVint STR;
            SVint DEX;
            SVint LUK;
            SVint WIS;

        } Info;
    } MDBHeroStatUnion;

    MDBHeroStatUnion Stat;

    int GetSequence() const { return Seq; }
    bool IsValid() const;

};