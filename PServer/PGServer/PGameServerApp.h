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
     *  서버에서 발생하는 로그 데이터를 기록하기 위한 세팅 함수.
     */
    void _InitLog();

    bool _InitGameDB();

    bool _LoadMasterDB();

    bool _InitMap();
};

