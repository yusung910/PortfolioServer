#pragma once
#pragma warning(push)
#pragma warning(disable : 26812)
#include <Protocol_generated.h>
#pragma warning(pop)

#include <RefSingleton.h>
#include <DefineMacro.h>

// InnerPacket Pointer nullptr üũ
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
     *  _isAllowEmpty = true -> _dest�� ���� ����
     *  _isAllowEmpty = false -> _dest�� ���� �״�� �д�
     */
    bool CheckAndSetStr(const flatbuffers::String* _src, std::string& _dest, const bool& _isAllowEmpty = false);

    void SendError(int _hostID, EPacketProtocol _srcProtocol, EErrorMsg _msg);

    int64_t GetGroupSeq();


};

