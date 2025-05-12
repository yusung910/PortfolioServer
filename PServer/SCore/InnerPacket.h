#pragma once
#include <PGObject.h>

// InnerPacket�� ����ϱ� ���� �θ� Ŭ����, ����Ͽ� ����Ѵ�
class InnerDataInterface
{
public:
    InnerDataInterface() = default;
    virtual ~InnerDataInterface()
    {
        Release();
    }

    virtual void Release() {}; // �����ϴ� �Լ�
};


class InnerPacket : public PGObject<InnerPacket>
{
public:
    int m_nHostID = 0;
    int m_nProtocol = 0;    //� DB �۾����� MessageID 

    InnerDataInterface* m_pData = nullptr;

    int m_nWorkerID = 0;    // �ε�뷱���� ���Ǵ� Ű��, ��� ����ID, AccountUniqueID �� �б⿡ ���Ǵ� ���� ��

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