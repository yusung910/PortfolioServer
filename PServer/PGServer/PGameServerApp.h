#pragma once
class PGameServerApp
{
public:
    PGameServerApp();
    virtual ~PGameServerApp();


    /*!
     *  Initializes the game server app.
     *
     *      @return 
     */
    bool Initialize();


    bool RunLoop();
    //void SendServerLog(const std::wstring& _remark);

private:
    /*!
     *  �������� �߻��ϴ� �α� �����͸� ����ϱ� ���� ���� �Լ�.
     */
    void _InitLog();
};

