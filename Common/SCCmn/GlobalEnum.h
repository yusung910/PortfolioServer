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

//���� ���� ���� ���� ���� ��Ȳ
namespace EServerStatus
{
    enum Type :int
    {
        None = 0,

        Maintain,   //���� ��
        Good,       //���� -> ���� ���� 
        Confusion,  //ȥ�� => �߰� ���� 
        Overflow,   //��ȭ -> ���� ���� 
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

// ���� ����
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

//���� Ŭ�� ����
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

//Login ����
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

//���� ����
namespace EBuild
{
    enum Type
    {
        None = 0,
        Dev = None,     //����
        Live = 1,       //����
    };
}

//��������
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

//���� ����
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
     *  MasterDB-> MDBPilgrimStat ���� �ʿ�
     *******************************************************************/
    enum Type
    {
        None = 0,
        Level,            //��
        Strength,         //��
        Dexterity,        //��ø
        Endurance,        //�γ�
        WeaponMastery,    //���� ���õ�

        //
        HP,               //���ɷ�
        MP,               //����

        //
        MaxHP,            //�ִ�����
        MaxMP,            //�ִ븶��

        //
        MeleeMinDamage,   // ���� �ּ� ������
        MeleeMaxDamage,   // ���� �ִ� ������
        MeleeDamage,      // ���� ���ݷ�
        MeleeDefence,     // ���� ����
        
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
        case Level:
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

//���� ���õ� ����
namespace EWeaponMastery
{
    enum Type
    {
        None = 0,
        Sword,      //��
        blade,      //��
        Spear,      //â
        Clow,       //��

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

//Map District Type
namespace EDistrict
{
    enum Type
    {
        None = 0,   //�Ϲ�����
        Safe,       //����
        Training,   //�Ʒ�
        PK,         //PVP
        ClanWar,    //�����������
        

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


//��� ����
namespace EEquipSlot
{
    enum Type
    {
        None = 0,

        //basic
        Amulet,         //�����
        Armor,          //����
        Shirts,         //����
        Pants,          //����
        Cape,           //����
        Weapon,         //����
        Shield,         //��ȣ
        Shoes,          //�Ź�
    };

    inline bool IsBasicEquipSlot(const Type& _t)
    {
        return(_t >= Amulet && _t <= Shoes);
    }
}

//���� ���
namespace TransformGrade
{
    enum Type
    {
        None = 0,
        First,
        Second,
        Third,
        Fourth,
        fifth
    };
}

//���� �ܰ�
namespace AwakenGrade
{
    enum Type
    {
        None = 0,
        _1st,
        _2nd,
        _3th

    };
}

//NPC ����
namespace ENPC
{
    enum Type : int
    {
        None = 0,

        Dummy,   // ���� ������ ���� NPC -> ����, ������ ���
        Monster, // ��� ����
        Summon,  // ��ȯ��

        Supporter,  //������ NPC -> ���� ���

        Max
    };
}

//NPC ���
namespace ENPCMonsterGrade
{
    enum Type : int
    {
        None = 0,
        Normal,         //�Ϲ� ����
        Named,          //���ӵ� ����
        Boss,           //���� ����
        Max
    };
}

//NPC Monster Group
namespace ENPCMonsterGroup
{
    enum Type : int
    {
        None = 0,

        Normal,         //�Ϲ�
        Beast,          //�߼�
        Human,          //�ΰ�
        Demon,          //�Ǹ�
        Zombi,          //����
        Max
    };
}


//Supporter NPC Group
namespace ENPCSupporterGroup
{
    enum Type : int
    {
        None = 0,
        Story,      //���丮
        Inn,        //����
        Store,      //����
        WareHouse,  //â��
        Max
    };
}