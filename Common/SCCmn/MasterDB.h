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

//�⺻���ΰ͸� ����, ���� �߰��Ǵ� �������� ���� �۾� �ʿ�
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