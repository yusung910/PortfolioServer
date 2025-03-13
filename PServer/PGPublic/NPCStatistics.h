#pragma once

class NPC;
struct MDBNPC;
struct MDBObjectStatistics;

/*!
 *  NPC 스탯 정보 모듈 클래스
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

