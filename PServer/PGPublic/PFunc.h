#pragma once
#pragma warning(push)
#pragma warning(disable : 26812)
#include <Protocol_generated.h>
#pragma warning(pop)

#include <RefSingleton.h>
#include <DefineMacro.h>

// InnerPacket Pointer nullptr 체크
#define CheckPointer()                                          \
if(nullptr == _data.get())                                      \
{                                                               \
    SafeDelete(_data->m_pData);                                 \
    return false;                                               \
}                                                               \
if(nullptr == _data->m_pData)                                   \
    return false;                                               \

// DB Session
#define GetDBSession()                                          \
auto lDBSess = m_oDBWorker.GetSession();                        \
if(nullptr == lDBSess)                                          \
    return false;                                               \
Poco::Data::Session& lSess = *lDBSess;                          \



class PFunc : public RefSingleton<PFunc>
{
private:

public:
    PFunc();
    virtual ~PFunc();

    bool CheckHasStr(const flatbuffers::String* _src);

    /*!
     *  _isAllowEmpty = true -> _dest에 값을 비운다
     *  _isAllowEmpty = false -> _dest의 값을 그대로 둔다
     */
    bool CheckAndSetStr(const flatbuffers::String* _src, std::string& _dest, const bool& _isAllowEmpty = false);

    void SendError(int _hostID, EPacketProtocol _srcProtocol, EErrorMsg _msg);

    int64_t GetGroupSeq();


};

