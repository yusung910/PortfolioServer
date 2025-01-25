#include "PGPPrivate.h"
#include "ServerConst.h"

#include <DBService.h>

ServerConst::ServerConst()
{
}

const int ServerConst::GetInt(const EServerConst& _id) const
{
    auto item = m_mConstIntMap.find(_id);
    if (item != m_mConstIntMap.end())
        return item->second;

    return 0;
}

const std::wstring* ServerConst::GetString(const EServerConst& _id) const
{
    auto item = m_mConstStringMap.find(_id);
    if (item != m_mConstStringMap.end())
        return &item->second;
    return nullptr;
}

bool ServerConst::LoadServerConst(DBService* _service)
{
    if (nullptr == _service)
        return false;

    auto lTmpSess = _service->GetSession();

    if (nullptr == lTmpSess)
        return false;


    Poco::Data::Session& lSess = *lTmpSess;
    Poco::Data::Statement lStmt(lSess);

    std::wstring lStrVal = {};
    int lType = 0;

    BEGIN_SESSION
        lStmt << "{ CALL spServerConfigList }"
            , into(lType)
            , into(lStrVal)
            , range(0, 1)
            , now;
    END_SESSION;

    while (!lStmt.done())
    {
        if (lStmt.execute() > 0)
        {
            EServerConst lId = static_cast<EServerConst>(lType);
            if (_IsIntConst(lId))
            {
                try
                {
                    int lVal = _wtoi(lStrVal.c_str());
                    m_mConstIntMap[lId] = lVal;
                }
                catch (...)
                {
                    VIEW_WRITE_ERROR(L"Invalid ServerConst Value!! Key(%d) Value(%s)", lId, lStrVal.c_str());
                }
            }
            else
            {
                m_mConstStringMap[lId] = lStrVal;
            }
        }
    }

    return true;
}

void ServerConst::SetDefault()
{
}

bool ServerConst::_IsIntConst(const EServerConst& _id)
{
    //switch (_id)
    //{

    //default:
    //    break;
    //}
    
    
    return true;
}
