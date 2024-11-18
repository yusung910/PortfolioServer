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
    void _RegisterPacketHandlers();	// ��Ŷ �ڵ鷯 ��� ����
    void _RegisterTimers();			// RegisterTimer (��Ŷ ����) ó�� ��� ����
    void _RegisterSchedules();		// Schedule (��Ŷ �񵿱�) ó�� ��� ����
};

