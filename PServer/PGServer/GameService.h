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

    //패킷 병렬화 실패 처리
    virtual void OnDeserializeFail(const int& _hostID, const std::string& _hostIP, const int& _failProtocol) override;

protected:
    //======================================================================
    //Packet Process Area Begin
    //======================================================================

    //system
    bool OnHostConnect(int _hostID, const HostConnect& _msg);
    bool OnHostClose(int _hostID, const HostClose& _msg);


    //Game

    //======================================================================
    //Packet Process Area end
    //======================================================================
private:
    void _RegisterPacketHandlers();	// 패킷 핸들러 등록 모음
    void _RegisterTimers();			// RegisterTimer (패킷 동기) 처리 등록 모음
    void _RegisterSchedules();		// Schedule (패킷 비동기) 처리 등록 모음
};

