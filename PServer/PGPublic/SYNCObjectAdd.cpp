#include "PGPPrivate.h"
#include "SYNCObjectAdd.h"

SYNCObjectAdd::SYNCObjectAdd()
{
}

SYNCObjectAdd::~SYNCObjectAdd()
{
}

void SYNCObjectAdd::Reset()
{
    SYNCObject::Reset();
}

void SYNCObjectAdd::Set(BaseObject& _bo)
{
    SYNCObject::Set(_bo);
    m_fPosX = _bo.GetPosition().x;
    m_fPosY = _bo.GetPosition().y;
    m_nDegree = _bo.GetDirection().GetYaw360();
    m_sName = _bo.GetName();
}

void SYNCObjectAdd::Get([[maybe_unused]] flatbuffers::FlatBufferBuilder& _fb, [[maybe_unused]] std::vector<flatbuffers::Offset<DSync>>& _list)
{

}
