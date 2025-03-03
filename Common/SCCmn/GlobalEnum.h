#pragma once
// C++17 MSVC Guideline Warning (enum)
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26812)
#endif
// end, C++17 MSVC Guideline Warning (enum)

#ifndef FOW_SERVER_BUILD
#	ifndef CLIENT_ONLY
#		define CLIENT_ONLY
#	endif
#endif


namespace EServer
{
    enum Type
    {
        None = 0,
        CDN,
        Login,	        // 로그인 서버
        Game,	        // 게임 서버
        Log,	        // 로그 서버
        Platform,       // 플랫폼 (원격관리) 서버
        Messenger,      // 메신저 서버
        API,	        // API 서버
        Bill,
        Max,
    };

    inline bool IsValid(const Type& t)
    {
        if (t <= None || t >= Max)
            return false;
        return true;
    }
}

//서버 내부 유저 수에 따른 상황
namespace EServerStatus
{
    enum Type :int
    {
        None = 0,

        Maintain,   //점검 중
        Good,       //정상 -> 적은 부하 
        Confusion,  //혼잡 => 중간 부하 
        Overflow,   //포화 -> 높은 부하 
        Max,
    };

    inline bool IsServerOn(const Type& _t)
    {
        switch (_t)
        {
        case Good:
        case Confusion:
        case Overflow:
            return true;
        default:
            break;
        }

        return false;
    }
};

// 서버 상태
namespace EServerState
{
    enum Type :int
    {
        None = 0,
        Recommend = 1,
        CreateLimit = 2,
        Max
    };
}

//접속 클라 유형
namespace EClient
{
    enum Type :int
    {
        None = 0,
        Windows = None,

        Android = 1,
        iOS = 11,
        WindowsUniversal = 21,

        Max
    };
}

//Login 유형
namespace ELoginPlatform
{
    enum Type
    {
        None = 0,
        Google = 1,
        Naver,
        AppStore,
        Facebook,
        Guest = 99,
        Max
    };

    inline bool IsGuestPlatform(const Type& _t)
    {
        return (_t == Guest);
    }
}

//빌드 유형
namespace EBuild
{
    enum Type
    {
        None = 0,
        Dev = None,     //개발
        Live = 1,       //배포
    };
}

//계정유형
namespace EAccount
{
    enum Type
    {
        None = 0,
        QA,
        GM,
        User = 99
    };
}

//계정 상태
namespace EAccountStatus
{
    enum Type
    {
        None = 0,
        Active = 1,
        EternalBan = 4,
        PeriodBan = 5,
        DeleteWaiting = 8,
        Max
    };
}

namespace EAbility
{
    /*******************************************************************
     *  MasterDB-> MDBPilgrimStat 선언 필요
     *******************************************************************/
    enum Type
    {
        None = 0,
        Strength,         //힘
        Dexterity,        //민첩
        Endurance,        //인내
        WeaponMastery,    //무기 숙련도

        //
        HP,               //생령력
        MaxHP,            //최대생명력
        MP,               //마나
        MaxMP,            //최대마나

        //
        MeleeMinDamage,   // 근접 최소 데미지
        MeleeMaxDamage,   // 근접 최대 데미지
        MeleeDamage,      // 근접 공격력
        MeleeDefence,     // 근접 방어력
        
        Max
    };

    inline bool IsValid(Type _t)
    {
        if (_t < None || _t >= Max)
            return false;
        return true;
    }

    inline bool IsTrainingStat(Type _t)
    {
        switch (_t)
        {
        case Strength:
        case Dexterity:
        case Endurance:
        case WeaponMastery:
            return true;
            break;
        }

        return false;
    }

}

//무기 숙련도 종류
namespace EWeaponMastery
{
    enum Type
    {
        None = 0,
        Sword,      //검
        blade,      //도
        Spear,      //창
        Clow,       //조

        Max,
    };
}

//MapType
namespace EMap
{
    enum Type
    {
        None = 0,
        Town,
        HuntingZone,
        BattleField,
        Max
    };
}

//CostType
namespace ECost
{
    enum Type
    {
        None = 0,
        Gold,
        Cash,
        Max
    };
}



namespace EEquipSlot
{
    enum Type
    {
        None = 0,

        //basic
        Amulet,         //목걸이
        Armor,          //갑옷
        Shirts,         //상의
        Pants,          //하의
        Cape,           //망토
        Weapon,         //무기
        Shield,         //슬호
        Shoes,          //신발
    };

    inline bool IsBasicEquipSlot(const Type& _t)
    {
        return(_t >= Amulet && _t <= Shoes);
    }
}