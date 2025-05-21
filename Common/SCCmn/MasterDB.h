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
struct MDBObjectStatistics;

struct MDBObjectStatistics
{
    SVint ObjectStatID;

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
    } MDBObjectStatisticsUnion;

    MDBObjectStatisticsUnion Stat;

    int GetObjectStatID() const { return ObjectStatID; }

    bool IsValid() const;

};


//맵 정보
struct MDBMapInfo
{
    SVint MapID = 0;
    SVint ParentMapID = 0;
    SVString MapFileName;
    SVString MapFilePath;

    SVint MapSize = 0;

    SVint MaxChannelCount = 0;
    SVint Channel = 0;

    EMap::Type MapType = EMap::None;

    SVint ActiveYN = 1;
    SVint SafeZoneYN = 1;
    SVint TeleportableYN = 1;

    bool GetActiveYN() const { return (ActiveYN == 1); }
    bool GetSafeZoneYN() const { return (SafeZoneYN == 1); }
    bool GetTeleportableYN() const { return (TeleportableYN == 1); }
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

//맵 구역 정보
struct MDBMapDistrict
{
    SVint MapDistrictID = 0;
    SVint MapID = 0;
    SVint TransportNPCID = 0;
    SVint MapDistrictGroup = 0;
    SVint MapDistrictType = 0;
    SVString MapDistrictName = 0;
    SVbool TeleportEnterYN = true;
    SVbool TeleportEscapeYN = true;
};


//레벨 별 기본 스탯
struct MDBPilgrimLevelBaseStat
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


//훈련스탯(힘,민,맷,숙)에 따른 스탯 비율
struct MDBAbilityStatRatePerTrainingStat
{
    SVint StatNo = 0;
    SVString Name = "";
    SVString Remark = "";
    SVint Strength = 0;
    SVint Dexterity = 0;
    SVint Endurance = 0;
    SVint WeaponMastery = 0;
};

//각성에 따른 A값
struct MDBAwakenAValue
{
    SVint AwakenSeq = 0;
    SVint AValue = 0;
};


//NPC 테이블
struct MDBNPC
{
    SVint NPCUniqueID = 0;
    SVString Name = "";
    SVint NPCType = 0;
    SVint MonsterGrade = 0;
    SVint MonsterGroup = 0;
    SVint SupporterGroup = 0;
    SVint Level = 0;
    SVint Exp = 0;

    const MDBObjectStatistics* ObjectStat = nullptr;

};

//NPCSummon 테이블
struct MDBNPCSummons
{
    SVint MapID = 0;
    SVint NPCUniqueID = 0;
    SVString SummonName = "";
    SVint SummonGroup = 0;
    SVint ResummonTime = 0;
    SVint Rotation = 0;

};