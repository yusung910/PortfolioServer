#pragma once
class PGameServerApp
{
public:
    PGameServerApp();
    virtual ~PGameServerApp();

    bool Initialize();
    bool RunLoop();
    void SendServerLog(const std::wstring& _remark);

private:
    void _InitLog();
};

