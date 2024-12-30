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

//서버 밀도 -> 낮을 경우(Low) 추천
// 높을 경우 비추천
// 심하게 높을 경우 제한
namespace EServerDensity
{
    enum Type :int
    {
        None = 0,
        Low = 1,
        High = 2,
        Max,
    };
}