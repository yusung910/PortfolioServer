#include "MapDataManager.h"

#include "DataProcessManager.h"
#include "MapElementsData.h"
#include "MDBDataManager.h"
#include "PGPPrivate.h"
#include "ServerConfig.h"

#include <FileSystem.h>
#include <json/json.h>
#include <LoggingMacro.h>
#include <MasterDB.h>


MapDataManager::~MapDataManager()
{
    m_oMapFileList.clear();
    for (auto& map : m_umMapElementsDatas)
    {
        for (auto& mapElem : map.second)
        {
            SafeDelete(mapElem.second);
        }
        map.second.clear();
    }
    m_umMapElementsDatas.clear();
}

bool MapDataManager::Initialize()
{
    bool localSuccess = true;

    m_oMapFileList.clear();
    m_umMapElementsDatas.clear();

    //ServerConfig.json파일에 저장된 맵 파일 경로를 호출한다
    if (true == m_sMapFilePath.empty())
        m_sMapFilePath = ServerConfig::GetInst().GetConfig().GetMapInfoPath();

    //
    auto& localMaps = MDBDataManager::GetInst().GetAllMapInfoList();
    if (true == localMaps.empty())
    {
        VIEW_WRITE_ERROR("MDB MapInfo is Empty!!");
        return false;
    }

    for (auto it = localMaps.begin(); it != localMaps.end(); ++it)
    {
        if (it->first == 0) continue; // 0번 row는 걸러냄
        
        if (nullptr == it->second) continue;


        if (false == _AddData(it->second))
        {
            localSuccess = false;
        }
        
        switch (it->second->MapType)
        {
        default:

        }

    }

    return !m_oMapFileList.empty() && localSuccess;
}

bool MapDataManager::_AddData(MDBMapInfo* _info)
{
    if (nullptr == _info)
        return false;

    if (false == _info->GetActiveYN())
        return false;

    for (auto& lData : m_oMapFileList)
    {
        if (lData.m_nMapID == _info->MapID)
            return false;
    }

    MapLoadData addMapElemData;
    addMapElemData.m_nMapID = _info->MapID;
    addMapElemData.m_nParentMapID = _info->ParentMapID;

    addMapElemData.m_eMapType = _info->MapType;

    addMapElemData.m_nWidth = addMapElemData.m_nHeight = 0;

    //MapInfo(MDB)에 저장되어 데이터 값(MapFileName)으로
    //각각 맵 세팅에 필요한 json, bin 파일 + 확장자를 세팅한다
    addMapElemData.m_sNaviFileName = _info->MapFileName;
    addMapElemData.m_sNaviFileName.append(MAP_NAVI_FILE_EXT);

    addMapElemData.m_sMapInfoFileName = _info->MapFileName;
    addMapElemData.m_sMapInfoFileName.append(MAP_INFO_FILE_EXT);

    addMapElemData.m_sSpawnFileName = _info->MapFileName;
    addMapElemData.m_sSpawnFileName.append(MAP_SPAWN_FILE_EXT);

    if (false == _SetMapInfo(addMapElemData))
        return false;

    m_oMapFileList.push_back(addMapElemData);

    return true;
}

bool MapDataManager::_SetMapInfo(MapLoadData& _data)
{
    //MapInfo파일 호출
    std::string lMapInfoFileName = FileSystem::GetExecutableDirName() + m_sMapFilePath + _data.m_sMapInfoFileName;

    //
    std::ifstream lIS(lMapInfoFileName);

    //MapInfoFile(.map.json)이 없을 경우 spawn.json파일 호출
    if (false == lIS.is_open())
    {
        lIS.close();
        lMapInfoFileName = m_sMapFilePath + _data.m_sSpawnFileName;
        lIS = std::ifstream(lMapInfoFileName);
        if (false == lIS.is_open())
        {
            //해당 MapID과 관련된 json 파일(info, spawn 등)이 없음
            VIEW_WRITE_ERROR(L"Map(ID: %d) has not Json File!", _data.m_nMapID);
            return false;
        }
    }

    size_t lFileSize = (size_t)std::filesystem::file_size(lMapInfoFileName.c_str());
    std::string lStrBuffer;
    lStrBuffer.reserve(lFileSize);

    std::string lStrLine;
    while (getline(lIS, lStrLine))
        lStrBuffer.append(lStrLine);

    lIS.close();

    Json::CharReaderBuilder lBuilder;
    const std::unique_ptr<Json::CharReader> lReader(lBuilder.newCharReader());

    Json::Value lRoot;
    Json::Value lSubRoot;

    JSONCPP_STRING lErr;
    
    if (false == lReader->parse(lStrBuffer.c_str(), lStrBuffer.c_str() + lStrBuffer.length(), &lRoot, &lErr))
        return false;

    float lLeft = 0.0f;
    float lBottom = 0.0f;
    float lWidth = 0.0f;
    float lHeight = 0.0f;

    int lSize = lRoot.size();
    if (1 != lSize)
        return false;

    if (auto lJsonData = lRoot[0]["left"]; false == lJsonData.isNull())
        lLeft = lJsonData.asFloat();

    if (auto lJsonData = lRoot[0]["bottom"]; false == lJsonData.isNull())
        lBottom = lJsonData.asFloat();

    if (auto lJsonData = lRoot[0]["width"]; false == lJsonData.isNull())
        lWidth = lJsonData.asFloat();

    if (auto lJsonData = lRoot[0]["height"]; false == lJsonData.isNull())
        lHeight = lJsonData.asFloat();

    _data.m_oCenterPosition.Set(lLeft + (lWidth * 0.5f), lBottom + (lHeight * 0.5f, 0.0f));

    _data.m_nWidth = static_cast<int>(std::ceilf(lWidth));
    _data.m_nHeight = static_cast<int>(std::ceilf(lHeight));

    int lTmpWidth = (int)lWidth;
    int lTmpHeight = (int)lHeight;

    if (lTmpWidth <= 0 || lTmpHeight <= 0)
        return false; //맵 사이즈가 0보다 작다?? 에러!

    std::map<int, std::string> lElementsMap;
    auto& lElements = lRoot[0]["elements"];

    for (int i = 0; i < (int)lElements.size(); i++)
    {
        std::string lName = StringUtil::ToMultiByte(StringUtil::UTF8_WSTR(lElements[i]["name"].asString()));
        lElementsMap.emplace(i, lName);
    }


    //이게 왜 필요한지는 모르겠네.. 나중에 UE에서 사용될 경우 작업
    //auto& lLayers = lRoot[0]["layers"];

    //for (int nLayerID = 0; nLayerID < (int)layers.size(); nLayerID++)
    //{
    //    int index = 0;
    //    auto& in = layers[nLayerID]["materialIndicesRLE"];

    //    std::string layerName = StrUtil::ToMultiByte(StrUtil::UTF8_WSTR(layers[nLayerID]["name"].asString()));
    //    if (layerName.find("nospawn") == 0)
    //    {
    //        // 레이어는 하나만 써야함.
    //        // 여러 레이어에 이렇게 만들 경우 여러 레이어 합치는 로직 만들어야함.
    //        mZoneNoSpawnLayerIDList.insert({ data.nZoneID,layers[nLayerID]["layerID"].asInt() });
    //    }


    //    for (int i = 0; i < (int)in.size(); i += 2)
    //    {
    //        int len = in[i].asInt();
    //        int val = in[i + 1].asInt();

    //        auto pData = _GetMapMaterialData(data.nZoneID, nLayerID);
    //        if (nullptr == pData)
    //        {
    //            pData = new MapMaterialData(nLayerID, nWidth, nHeight);
    //            if (false == _HasMapMaterialDataList(data.nZoneID))
    //            {
    //                std::unordered_map<int, MapMaterialData*> mapDatas;
    //                mapDatas.insert(std::pair(nLayerID, pData));
    //                mZoneMaterialDatas.insert(std::pair(data.nZoneID, mapDatas));
    //            }
    //            else
    //            {
    //                mZoneMaterialDatas[data.nZoneID].insert(std::pair(nLayerID, pData));
    //            }
    //        }

    //        int* pLayerOffset = pData->GetLayerDataBuffer() + index;
    //        for (int cnt = 0; cnt < len; ++cnt)
    //            pLayerOffset[cnt] = val;

    //        index += len;
    //    }
    //}

    int lMapID = _data.m_nParentMapID > 0 ? _data.m_nParentMapID : _data.m_nMapID;
    //auto& mapDistrict = MDBDatas::

    return true;
}

MapElementsData* MapDataManager::_GetMapElementsData(int _mapID, int _layerNum)
{
    auto lItMap = m_umMapElementsDatas.find(_mapID);

    if (lItMap == m_umMapElementsDatas.end())
        return nullptr;

    auto lItElem = lItMap->second.find(_layerNum);
    if (lItElem == lItMap->second.end())
        return nullptr;

    return lItElem->second;
}

bool MapDataManager::_HasMapElementsDataList(int _mapID)
{
    return m_umMapElementsDatas.end() != m_umMapElementsDatas.find(_mapID);
}
