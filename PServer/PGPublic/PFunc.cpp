#include "PGPPrivate.h"
#include "PFunc.h"
#include <NetworkManager.h>

PFunc::PFunc()
{
}

PFunc::~PFunc()
{
}

bool PFunc::CheckHasStr(const flatbuffers::String* _src)
{
    if (_src == nullptr || _src->size() == 0)
        return false;

    return true;
}

bool PFunc::CheckAndSetStr(const flatbuffers::String* _src, std::string& _dest, const bool& _isAllowEmpty)
{
    if (_isAllowEmpty)
    {
        if (_src == nullptr || _src->size() == 0)
            _dest = "";
        else
            _dest = _src->c_str();
    }
    else
    {
        if (_src == nullptr || _src->size() == 0)
            return false;
            
        _dest = _src->c_str();
    }

    return false;
}

void PFunc::SendError(int _hostID, EPacketProtocol _srcProtocol, EErrorMsg _msg)
{
    flatbuffers::FlatBufferBuilder lFbb;
    lFbb.Finish(CreateSCIntegrationErrorNotification(lFbb, (int)_srcProtocol, (int)_msg));

    Packet::SharedPtr lPacket = Packet::New();
    lPacket->SetPacketData(SC_IntegrationErrorNotification, lFbb.GetBufferPointer(), lFbb.GetSize());

    NetworkManager::GetInst().Send(_hostID, lPacket);

}

int64_t PFunc::GetGroupSeq()
{
    static int64_t lGrpSeq = Clock::GetFileTime();
    return lGrpSeq;
}
