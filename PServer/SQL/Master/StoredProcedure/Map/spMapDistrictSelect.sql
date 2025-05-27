/*************************************************************************************
    name        : (sp)[spMapDistrictSelect]
    description : 맵 구역
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.05.27.         유성            최초 생성





*************************************************************************************/
CREATE PROCEDURE [dbo].[spMapDistrictSelect]
AS
BEGIN
    SELECT    [MapDistrictID]
            , [MapID]
            , [TransportNPCID]
            , [MapDistrictGroup]
            , [MapDistrictType]
            , [MapDistrictName]
            , [TeleportEnterYN]
            , [TeleportEscapeYN]
      FROM MapDistrict
END