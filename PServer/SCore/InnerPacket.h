#pragma once
#include <PGObject.h>

// InnerPacket을 사용하기 위한 부모 클래스, 상속하여 사용한다
class InnerDataInterface
{
public:
    InnerDataInterface() = default;
    virtual ~InnerDataInterface()
    {
        Release();
    }

    virtual void Release() {}; // 해제하는 함수
};


class InnerPacket : public PGObject<InnerPacket>
{
public:
    int m_nHostID = 0;
    int m_nProtocol = 0;    //어떤 DB 작업인지 MessageID 

    InnerDataInterface* m_pData = nullptr;

    int m_nWorkerID = 0;    // 로드밸런스에 사용되는 키값, 대상 서버ID, AccountUniqueID 등 분기에 사용되는 구분 값

public:
    InnerPacket() : InnerPacket(ObjectTag()) {};
    InnerPacket(ObjectTag) {};

    virtual void Reset()
    {
        m_nHostID = 0;
        m_nProtocol = 0;
        m_nWorkerID = 0;
        if (m_pData != nullptr)
        {
            delete m_pData;
            m_pData = nullptr;
        }
    }
};