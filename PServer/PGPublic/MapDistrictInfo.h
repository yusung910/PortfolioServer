#pragma once
#include <unordered_map>
#include <GlobalEnum.h>

struct MDBMapDistrict;

class MapDistrictInfo
{
public:
    bool m_bIsForcedDistrict = false;
    EDistrict::Type m_eDistrictType = EDistrict::None;
    std::unordered_map<int, MDBMapDistrict> m_umMapDistrictTypeMap;

public:
    MapDistrictInfo() = default;
    ~MapDistrictInfo() = default;
};