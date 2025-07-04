#include "PGPPrivate.h"
#include "MDBDataLoader.h"
#include "PFunc.h"
#include "PocoTimeUtil.h"
#include <TimeUtil.h>

using namespace Poco::Data::ODBC;

bool MDBDataLoader::SetConfig(const std::string& _userID, const std::string& _password, const std::string& _database, const std::string& _host, const std::string& _port)
{
    m_oDBWorker.SetDBConfig(
        "SQL Server", _userID, _password, _database, _host, _port
    );

    if (false == m_oDBWorker.Init())
    {
        VIEW_WRITE_ERROR(L"DB Initialize Failed: %s / DB: %s", _host.c_str(), _database.c_str());
        return false;
    }

    return true;
}

bool MDBDataLoader::LoadMDBDatas(MDBDatas& _datas)
{
    if (false == m_oDBWorker.IsConnected())
        return false;

    _datas.Reset();

    if (false == _ReadAbilityComponents(_datas)) return false;
    if (false == _ReadAbilityModule(_datas)) return false;

    if (false == _ReadMapInfo(_datas)) return false;

    return true;
}

bool MDBDataLoader::_ReadAbilityComponents(MDBDatas& _datas)
{
    GetDBSession();

    bool lRslt = true;
    try
    {
        MDBAbilityComponents lTmp{};

        Poco::Data::Statement lSelect(lSess);

        lSelect << "{CALL spAbilityComponentsSelect}"
            , into(lTmp.AbilityComponentSeq)
            
            , range(0, 1);

        //bool done()
        //Returns true if the statement was completely executed or false if a range limit stopped it and there is more work to do.When no limit is set, it will always return true after calling execute().
        while (!lSelect.done())
        {
            if (lSelect.execute() > 0)
            {
                MDBAbilityComponents* lAdd = new MDBAbilityComponents;
                memcpy_s(lAdd, sizeof(MDBAbilityComponents), &lTmp, sizeof(MDBAbilityComponents));
                auto lRet = _datas.GetAllMDBAbilityComponentsList().insert({ lAdd->AbilityComponentSeq, lAdd });

                //SEQ 중복체크
                if (!lRet.second)
                {
                    VIEW_WRITE_ERROR("AbilityComponents Sequence: %d, IS NULL", lRet.first->first);
                    lRslt = false;
                }
            }
        }

    }
    catch (StatementException& _stateEx)
    {
        VIEW_WRITE_ERROR("\nException :: %s\nMessage :: %s", StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_stateEx.what())).c_str(), StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_stateEx.message().c_str())).c_str());
        return false;
    }
    catch (std::exception& _ex)
    {
        VIEW_WRITE_ERROR("\nDB Error :: %s", StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_ex.what())).c_str());
        return false;
    }

    return lRslt;
}

bool MDBDataLoader::_ReadAbilityModule(MDBDatas& _datas)
{
    GetDBSession();

    bool lRslt = true;
    try
    {
        MDBAbilityModule lTmp{};

        Poco::Data::Statement lSelect(lSess);

        lSelect << "{CALL spAbilityModuleSelect}"
            , into(lTmp.AbilityModuleSeq)
            , into(lTmp.AbilityComponentsSeq)
            , into(lTmp.Value)

            , range(0, 1);

        //bool done()
        //Returns true if the statement was completely executed or false if a range limit stopped it and there is more work to do.When no limit is set, it will always return true after calling execute().
        while (!lSelect.done())
        {
            if (lSelect.execute() > 0)
            {
                MDBAbilityModule* lAdd = new MDBAbilityModule;
                memcpy_s(lAdd, sizeof(MDBAbilityModule), &lTmp, sizeof(MDBAbilityModule));
                auto lRet = _datas.GetAllMDBAbilityModuleList().insert({ lAdd->AbilityModuleSeq, lAdd });

                if (!lRet.second)
                {
                    VIEW_WRITE_ERROR("AbilityModule Sequence: %d, IS Null", lRet.first->first);
                    lRslt = false;
                }
            }
        }

    }
    catch (StatementException& _stateEx)
    {
        VIEW_WRITE_ERROR("\nException :: %s\nMessage :: %s", StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_stateEx.what())).c_str(), StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_stateEx.message().c_str())).c_str());
        return false;
    }
    catch (std::exception& _ex)
    {
        VIEW_WRITE_ERROR("\nDB Error :: %s", StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_ex.what())).c_str());
        return false;
    }

    return lRslt;
}

bool MDBDataLoader::_ReadMapInfo(MDBDatas& _datas)
{
    GetDBSession();

    bool lRslt = true;

    try
    {
        MDBMapInfo lTmp{};

        Poco::Data::Statement lSelect(lSess);
        int localMapType = 0;
        lSelect << "{CALL spMapInfoSelect}"
            , into(lTmp.MapID)
            , into(lTmp.ParentMapID)
            , into(localMapType)
            , into(lTmp.MapFileName)
            , into(lTmp.MapFilePath)
            , into(lTmp.MapSize)
            , into(lTmp.ActiveYN)
            , into(lTmp.SafeZoneYN)
            , into(lTmp.TeleportableYN)
            , range(0, 1);

        //bool done()
        //Returns true if the statement was completely executed or false if a range limit stopped it and there is more work to do.When no limit is set, it will always return true after calling execute().
        while (!lSelect.done())
        {
            if (lSelect.execute() > 0)
            {
                lTmp.MapType = (EMap::Type)localMapType;
                MDBMapInfo* lAdd = new MDBMapInfo;
                memcpy_s(lAdd, sizeof(MDBMapInfo), &lTmp, sizeof(MDBMapInfo));
                auto lRet = _datas.GetAllMapInfoList().insert_or_assign(lTmp.MapID, lAdd);

                //SEQ 중복체크
                if (!lRet.second)
                {
                    VIEW_WRITE_ERROR("MDBMapInfo Sequence: %d, IS DUPLICATED", lRet.first->first);
                    lRslt = false;
                }
            }
        }

    }
    catch (StatementException& _stateEx)
    {
        VIEW_WRITE_ERROR("\nException :: %s\nMessage :: %s", StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_stateEx.what())).c_str(), StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_stateEx.message().c_str())).c_str());
        return false;
    }
    catch (std::exception& _ex)
    {
        VIEW_WRITE_ERROR("\nDB Error :: %s", StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(_ex.what())).c_str());
        return false;
    }

    return lRslt;
}
