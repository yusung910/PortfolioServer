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

    //��Ŷ ����ȭ ���� ó��
    virtual void OnDeserializeFail(const int& _hostID, const std::string& _hostIP, const int& _failProtocol) override;

protected:
    //======================================================================
    //Packet Process Area Begin
    //======================================================================

    //system
    bool OnHostConnect(int _hostID, const HostConnect& _msg);

    //Game
    bool OnCSAuthReq(int _hostID, const CSAuthReq& _msg);
    bool OnSCAuthReq(int _hostID, const CSAuthReq& _msg);

    //======================================================================
    //Packet Process Area end
    //======================================================================
private:
    void _RegisterPacketHandlers();	// ��Ŷ �ڵ鷯 ��� ����
    void _RegisterTimers();			// RegisterTimer (��Ŷ ����) ó�� ��� ����
    void _RegisterSchedules();		// Schedule (��Ŷ �񵿱�) ó�� ��� ����
};

