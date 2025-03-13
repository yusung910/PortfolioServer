#pragma once

class NPC;
struct MDBNPC;
struct MDBObjectStatistics;

/*!
 *  NPC ���� ���� ��� Ŭ����
 */
class NPCStatistics
{
private:
    NPC& m_oNPC;

    const MDBNPC* m_oMDBNPCData = nullptr;

public:
    NPCStatistics(NPC& _npc);
    ~NPCStatistics() = default;

    void Reset();

    bool SetMDBData(const MDBNPC* _data);
};

