#include "PGPPrivate.h"
#include "SYNCObject.h"
#include "BaseObject.h"

void SYNCObject::Reset()
{
    m_eObjectType = EObject::None;
    m_eBehavType = EObjectBehavior::None;

    m_nSharedCount = 0;
}

void SYNCObject::Set(BaseObject& _bo)
{
    m_nSeqID = _bo.GetSequence();
    m_eObjectType = _bo.GetObjectType();
    m_nSharedCount = 0;
}


void SYNCObject::SetBehaviorType(const EObjectBehavior::Type& _t) noexcept
{
    m_eBehavType = _t;
}

void SYNCObject::IncreaseSharedCount() noexcept
{
    ++m_nSharedCount;
}

const int& SYNCObject::DecreaseSharedCount() noexcept
{
    return --m_nSharedCount;
}

bool SYNCObject::IsNonShared() const noexcept
{
    return m_nSharedCount == 0;
}

const EObjectBehavior::Type& SYNCObject::GetBehaviorType() const noexcept
{
    return m_eBehavType;
}

const int& SYNCObject::GetSequenceID() const noexcept
{
    return m_nSeqID;
}

const EObject::Type& SYNCObject::GetObjectType() const noexcept
{
    return m_eObjectType;
}

bool SYNCObject::IsDirectSync(EObjectBehavior::Type _behav)
{
    return EObjectBehavior::IsDirectSync(_behav);
}

