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

    if (false == _ReadPilgrimStat(_datas))
        return false;

    return true;
}

bool MDBDataLoader::_ReadPilgrimStat(MDBDatas& _datas)
{
    GetDBSession();

    bool lRslt = true;
    try
    {
        MDBPilgrimStat lTmp;

        Poco::Data::Statement lSelect(lSess);

        lSelect << "{CALL spPilgrimStatSelect}"
            , into(lTmp.Seq)
            , into(lTmp.Stat.Info.Strength)
            , into(lTmp.Stat.Info.Dexterity)
            , into(lTmp.Stat.Info.Endurance)
            , into(lTmp.Stat.Info.WeaponMastery)
            , into(lTmp.Stat.Info.HP)
            , into(lTmp.Stat.Info.MaxHP)
            , into(lTmp.Stat.Info.MP)
            , into(lTmp.Stat.Info.MaxMP)
            , into(lTmp.Stat.Info.MeleeMinDamage)
            , into(lTmp.Stat.Info.MeleeMaxDamage)
            , into(lTmp.Stat.Info.MeleeDefence)
            , range(0, 1);

        //bool done()
        //Returns true if the statement was completely executed or false if a range limit stopped it and there is more work to do.When no limit is set, it will always return true after calling execute().
        while (!lSelect.done())
        {
            if (lSelect.execute() > 0)
            {
                MDBPilgrimStat* lAdd = new MDBPilgrimStat;
                memcpy_s(lAdd, sizeof(MDBPilgrimStat), &lTmp, sizeof(MDBPilgrimStat));
                auto lRet = _datas.GetAllPilgrimStatList().insert({ lAdd->GetSequence(), lAdd });

                //SEQ 중복체크
                if (!lRet.second)
                {
                    VIEW_WRITE_ERROR("PilgrimStat Sequence: %d, IS DUPLICATED", lRet.first->first);
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
        MDBMapInfo lTmp;

        Poco::Data::Statement lSelect(lSess);

        lSelect << "{CALL spMapInfoSelect}"
            , into(lTmp.MapID)
            , into(lTmp.ParentMapID)
            , into(lTmp.MapType)
            , into(lTmp.MapName)
            , into(lTmp.Mapsize)
            , into(lTmp.IsActive)
            , into(lTmp.IsSafeZone)
            , into(lTmp.IsTeleportable)
            , range(0, 1);

        //bool done()
        //Returns true if the statement was completely executed or false if a range limit stopped it and there is more work to do.When no limit is set, it will always return true after calling execute().
        while (!lSelect.done())
        {
            if (lSelect.execute() > 0)
            {
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
