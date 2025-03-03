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
struct MDBPilgrimStat;

struct MDBPilgrimStat
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

            SVint MeleeMinDamage;   //근접 최소 데미지
            SVint MeleeMaxDamage;   //근접 최대 데미지

            SVint MeleeDefence;     //근접 방어력

        } Info;
    } MDBPilgrimStatUnion;

    MDBPilgrimStatUnion Stat;

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

    bool GetIsActive() const { return (IsActive == 1); }
    bool GetIsSafeZone() const { return (IsSafeZone == 1); }
    bool GetIsTeleportable() const { return (IsTeleportable == 1); }
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


//레벨 별 기본 스탯
struct MDBPilgrimLevelBaseAbilityStat
{
    SVint Level = 0;

    SVint MaxHP = 0;
    SVint MaxMana = 0;
    SVint HPRegen = 0;
    SVint ManaRegen = 0;
    SVint Defence = 0;
    SVint MeleeDamageMin = 0;
    SVint MeleeDamageMax = 0;
};