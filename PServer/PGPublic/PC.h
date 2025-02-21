#pragma once
#include "BaseObject.h"
#include "GlobalEnum.h"
#include "GlobalConst.h"
#include "SimpleHeroInfo.h"


constexpr int64_t DEFAULT_AUTO_SAVE_TICK_MS = 1 * 60 * 1000;			// 2020.05.20. prym 1분으로 변경
constexpr int64_t DEFAULT_AUTO_SAVE_TICK_10MINUTES_MS = 10 * 60 * 1000;	// 10분에 한번씩 저장
constexpr int64_t DEFAULT_AUTO_SAVE_TICK_5MINUTES_MS = 5 * 60 * 1000;	// 5분에 한번씩 저장
class PC : public BaseObject, public PGObject<PC>
{
public:

private:

    std::string m_sOTP = "";
    int m_nHostID = 0;
    int m_nConnectIP = 0;

    EAccount::Type m_eAccountType = EAccount::None;

public:
    PC(ObjectTag);
    ~PC();

    virtual void Reset();
    virtual void Update(const int64_t& _deltaSec);
    void UpdateAttack(const int64_t& _deltaSec);

};

