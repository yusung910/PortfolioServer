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

}

void SYNCObjectAdd::Get(flatbuffers::FlatBufferBuilder& _fb, std::vector<flatbuffers::Offset<DSync>>& _list)
{
    SYNCObject::Get(_fb, _list);
}
