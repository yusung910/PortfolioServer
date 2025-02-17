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
            /*******************************************************************
             *  GlobalEnum -> EStats::Type에도 선언 필요
             *******************************************************************/
            SVint None;

            SVint Strength;         //힘
            SVint Dexterity;        //민첩
            SVint Endurance;        //인내
            SVint WeaponMastery;    //무기 숙련도

            SVint HP;               //생령력
            SVint MaxHP;            //최대생명력
            SVint MP;               //마나
            SVint MaxMP;            //최대마나


        } Info;
    } MDBHeroStatUnion;

    MDBHeroStatUnion Stat;

    int GetSequence() const { return Seq; }
    bool IsValid() const;

};


//맵 정보
struct MDBMapInfo
{
    SVint MapID = 0;
    SVint ParentMapID = 0;
    SVint MapType = 0;
    SVString MapName;

    SVint Mapsize = 0;
    SVint IsActive = 1;
    SVint IsSafeZone = 1;
    SVint IsTeleportable = 1;

    bool IsActive() const { return IsActive == 1; }
    bool IsSafeZone() const { return IsSafeZone == 1; }
    bool IsTeleportable() const { return IsTeleportable == 1; }
    EMap::Type GetMapType() const { return (EMap::Type)MapType; }
};

//맵 텔레포트 정보
struct MDBMapTeleportInfo
{
    SVint MapTeleportID = 0;
    SVint MapID = 0;
    SVint TeleportType = 0;
    SVint TeleportCostType = 0;
    SVint TeleportCostItemID = 0;

    //텔레포트 비용
    SVint TeleportCostValue = 0;

    SVfloat PosX = 0.0f;
    SVfloat PosY = 0.0f;
    SVfloat PosZ = 0.0f;

    ECost::Type GetTeleportCostType() const { return (ECost::Type)TeleportCostType; }
    
};