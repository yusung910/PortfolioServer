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
        Normal,     //정상 -> 적은 부하 
        Confusion,  //혼잡 => 중간 부하 
        Overflow,   //포화 -> 높은 부하 
        Max,
    };

    inline bool IsServerOn(const Type& _t)
    {
        switch (_t)
        {
        case Normal:
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
        GuestLogin = 99,
        Max
    };
}

//Store 결제
namespace EStore
{
    enum Type
    {
        None = 0,
        GooglePlay = 1,
        OneStore = 2,
        AppStore = 11,
        WindowsUniversal = 21,

        Max
    };
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


//계정 유형
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