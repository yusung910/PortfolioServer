#pragma once
#include "PGObject.h"
#include <unordered_set>
#include <map>
class Field;
class BaseObject;
class PC;
class SYNCObjectDUserBuff;

class Area : public PGObject<Area>
{
private:
    int m_nMapID = 0;
    EMap::Type m_eMapType = EMap::None;

    size_t m_nChannelCount = 0;
    size_t m_nMaxChannelCount = 0;

    Field* m_pField = nullptr;

    std::unordered_set<int> m_usHostIDList;


public:
    Area(ObjectTag);
    virtual ~Area();
    virtual void Reset() override;

    void SetMapID(const int& _mapID) noexcept;
    const int& GetMapID() const noexcept;

    void SetMapType(const EMap::Type& _t) noexcept;
    const EMap::Type GetMapType() const noexcept;

    bool Create(const int& _mapID, const int& _w, const int& _h);

};

