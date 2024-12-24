#pragma once
class PLoginServerApp
{
public:
    PLoginServerApp();
    virtual ~PLoginServerApp();


    /*!
     *  Initializes the game server app.
     *
     *      @return
     */
    bool Initialize();
    bool RunLoop();

private:
    /*!
     *  �������� �߻��ϴ� �α� �����͸� ����ϱ� ���� ���� �Լ�.
     */
    void _InitLog();

    bool _InitUserDB();
};

