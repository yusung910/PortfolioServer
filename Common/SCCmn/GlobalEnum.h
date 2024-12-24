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
        Login,	        // �α��� ����
        Game,	        // ���� ����
        Log,	        // �α� ����
        Platform,       // �÷��� (���ݰ���) ����
        Messenger,      // �޽��� ����
        API,	        // API ����
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

        Maintain,   //���� ��
        Normal,     //���� -> ���� ���� 
        Confusion,  //ȥ�� => �߰� ���� 
        Overflow,   //��ȭ -> ���� ���� 
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
}