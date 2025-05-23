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

struct MDBAbilityComponents
{
    SVint AbilityComponentSeq;

    //typedef union
    //{
    //    struct
    //    {
    //        /*******************************************************************
    //         *  GlobalEnum -> EStats::Type에도 선언 필요
    //         *******************************************************************/
    //        SVint None;

    //        SVint Level;


    //        SVint Strength;         //힘
    //        SVint Dexterity;        //민첩
    //        SVint Endurance;        //인내
    //        SVint WeaponMastery;    //무기 숙련도

    //        SVint CurrentHP;               //생령력
    //        SVint MaxHP;            //최대생명력
    //        SVint CurrentMP;               //마나
    //        SVint MaxMP;            //최대마나

    //        SVint MeleeMinDamage;   //근접 최소 데미지
    //        SVint MeleeMaxDamage;   //근접 최대 데미지

    //        SVint MeleeDefence;     //근접 방어력

    //        SVint CriticalRate;           //크리티컬 확률
    //        SVint CriticalDamageRate;     //크리티컬 대미지 배률

    //        SVint AttackSpeed;       //공격 속도
    //        SVint MovementSpeed;     //이동 속도

    //        SVint DodgeRate;      //회피율
    //        SVint BlockRate;      //방어율


    //    } Info;
    //} MDBAbilityComponentsUnion;

    //MDBAbilityComponentsUnion Stat;

    //int GetObjectStatID() const { return ObjectStatID; }

    //bool IsValid() const;

};

//아이템, 캐릭터 레벨 등등에 따른 AbilityModule
struct MDBAbilityModule
{
    SVint AbilityModuleSeq;
    SVint AbilityComponentsSeq;
    SVint Value;
};


//레벨 별 기본 스탯
struct MDBPilgrimLevelStatus
{
    SVint TrainingAbilityType;
    SVint Level = 0;
    SVint AbilityModuleSeq;
    SVint64 NeedNextLevelExp;
};


//각성에 따른 A값
struct MDBAwakenAValue
{
    SVint AwakenSeq = 0;
    SVint AValue = 0;
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

    const MDBAbilityModule* AbilityModule = nullptr;

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