#pragma once
#include <cstdint>
#include <string>

//Poco 관련 LNK2005가 발생할경우 아래 구문이 필요함
#ifndef POCO_STATIC
#define POCO_STATIC 1
#endif
//-----------------------------------------------
#include <Poco/Tuple.h>
#include <Poco/DateTime.h>

#pragma warning(push)
#pragma warning(disable : 26812)
#include <Protocol_generated.h>
#pragma warning(pop)

#include <GlobalEnum.h>
#include <GlobalConst.h>
#include <InnerPacket.h>

#include <array>
#include <unordered_set>

#include "PEnum.h"

//
class LoginAccountProcessSelectDTO : public InnerDataInterface
{
public:

    int Result = (int)EDBResult::None;
    int AccountSeq = 0;
    int AccountType = 0;
    int AccountStatus = 0;
    int LastConnectGameServerID = 0;

    Poco::DateTime DeleteRemainingPeriod = { 1900, 1, 1, 0, 0, 0 };

    int OTP = 0;

    int ClientType = 0;
    int AppVersion = 0;
    int LoginPlatformType = 0;
    std::string AccountUIDkey;
    
    int ConnectingLoginServerID = 0;

    int BuildType = 0;
    int IPAddress32 = 0;

    //
    std::unordered_set<int> HeroExistServerList;

public:
    virtual void Release() override;

    inline EPacketProtocol GetMsgID()
    {
        return EPacketProtocol::LUDB_AuthReq;
    }

};

class LoginAccountCharacterSelectDTO : public InnerDataInterface
{
public:
    typedef Poco::Tuple
        <
        int,                    // [CharacterSeq]
        int,                    // [ServerID]
        std::wstring,           // [CharacterName]
        int,                    // [CharacterLevel]
        int,                    // [CharacterClass]
        Poco::DateTime          // [LastTime]
        > AccountCharacter;

    int AccountSeq = 0;

    std::vector<AccountCharacter> characterList;

public:
    virtual void Release() override;

};