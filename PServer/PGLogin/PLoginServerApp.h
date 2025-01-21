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
     *  서버에서 발생하는 로그 데이터를 기록하기 위한 세팅 함수.
     */
    void _InitLog();

    bool _InitAccountDB();
};

