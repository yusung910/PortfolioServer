#pragma once
#include <cstdint>
#include <string>


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
#include <Poco/Tuple.h>
#include <Poco/DateTime.h>

//
class LoginAccountProcessSelectDTO : public InnerDataInterface
{
public:

    int Result = (int)EDBResult::None;
    int AccountSeq = 0;

    int LastConnectServerID = 0;

    Poco::DateTime SessionDate = { 1900, 1, 1, 0, 0, 0 };

    int OTP = 0;

    int ClientType = 0;
    int AppVersion = 0;
    int LoginPlatformType = 0;

    std::string AccountUIDkey;

    int IPAddress = 0;

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
        int,
        std::wstring,
        int,
        int,
        Poco::DateTime
        > AccountCharacter;

    int AccountSeq = 0;

    std::vector<AccountCharacter> characterList;

public:
    virtual void Release() override;

};