/*!
 *  @file LoginPlayerManager.h
 *  @author ShootingStar
 *  @date 2025-01-04
 *  @project PGLogin
 *
 *  LoginService �̿ܿ��� ���� ������ �� ó���� �� ��
 *  �� Ŭ���� ���ο��� �޾Ƽ� ������ ��
 *  ����� LoginPlayer*�� �޾Ƽ� ������� �ʵ��� �Ѵ�.
 */
#pragma once
#include <RefSingleton.h>
#include <unordered_map>
#include <mutex>

#include "LoginPlayer.h"

class LoginPlayerManager : public RefSingleton<LoginPlayerManager>
{
private:
    std::mutex m_xHostLock;
    std::unordered_map<int, LoginPlayer::SharedPtr> m_umPlayerList; //hostid, player

    std::unordered_map<int, int> m_umOTPList; //AccountSeq, OTP


public:
    LoginPlayerManager();
    virtual ~LoginPlayerManager();

    /*!
     *  �α��� ó���� �÷��̾ ����Ѵ�
     *  ��Ͽ� ���� ������� LoginPlayer ������ ������ ��ȯ
     *  �̹� ��� �Ǿ� ������� nullptr�� ��ȯ
     *
     *      @param [in,out] _hostID 
     *      @param [in]     _msg    
     *
     *      @return 
     */
    LoginPlayer* Add(int& _hostID, const CLAuthReq& _msg);
    void Remove(const int& _hostID);

    LoginPlayer* Find(const int& _hostID);
    LoginPlayer* FindByAccountSeq(const int& _accountSeq);

    size_t GetConnectPlayerCount();

    void GetOTP(const int& _accountSeq, int& _otp);
    void SetOTP(const int& _accountSeq, int& _otp);

public: // Get Data
    bool GetPlayerData(const int& _accountSeq, int& _hostID, int& _ServerID, std::string& _platformID, std::string& _returnToken);
    bool GetPlayerDataForDeque(const int& _hostID, int& _accountSeq, int& _ServerID, int& _otp, ELoginPlatform::Type& _loginPlatformType);
    bool SetServerID(const int& _hostID, const int& _serverID);

public: //network
    void Broadcast(const int& _msgID, flatbuffers::FlatBufferBuilder& _fbb);
    void Broadcast(const int& _msgID, void* _msg, const size_t& _msgSize);

    void SendPacket(const int& _hostID, const int& _msgID, flatbuffers::FlatBufferBuilder& _fbb);
    void SendPacket(const int& _hostID, const int& _msgID, void* _msg, const size_t& _msgSize);



};

