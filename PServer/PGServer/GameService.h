#pragma once
#include <RefSingleton.h>
#include <Service.h>

#include <Protocol_generated.h>
class GameService : public Service, public RefSingleton<GameService>
{
private:
    int m_nCurrentServerID = 10101;

    int m_nPoolCounter = 0;

    int m_nMaxConnection = 3000;
    bool m_bAllowConnect = true;

public:
    GameService();
    virtual ~GameService() = default;

    bool Start();

    void SetAllowConnect(bool _onoff);

private:
    void _RegisterPacketHandlers();	// 패킷 핸들러 등록 모음
    void _RegisterTimers();			// RegisterTimer (패킷 동기) 처리 등록 모음
    void _RegisterSchedules();		// Schedule (패킷 비동기) 처리 등록 모음
};

