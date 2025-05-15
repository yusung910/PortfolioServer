#pragma once
#include <GlobalEnum.h>

class BaseObject;

class SYNCObject
{
protected:
    EObject::Type m_eObjectType = EObject::None;
    EObjectBehavior::Type m_eBehavType = EObjectBehavior::None;

    int m_nSeqID = 0;
    int m_nSharedCount = 0;

public:
    SYNCObject() = default;
    virtual ~SYNCObject() = default;

    //------------------------------------------------------------------
    // Virtual Func
    virtual void Reset();
    virtual void Set(BaseObject& _bo);
    
    //------------------------------------------------------------------

    void SetBehaviorType(const EObjectBehavior::Type& _t) noexcept;
    void IncreaseSharedCount() noexcept;
    const int& DecreaseSharedCount() noexcept;
    bool IsNonShared() const noexcept;

    const EObjectBehavior::Type& GetBehaviorType() const noexcept;
    const int& GetSequenceID() const noexcept;
    const EObject::Type& GetObjectType() const noexcept;


    virtual void Get(flatbuffers::FlatBufferBuilder& _fb, std::vector<flatbuffers::Offset<DSync>>& _list) = 0;

    static bool IsDirectSync(EObjectBehavior::Type _behav);
};

