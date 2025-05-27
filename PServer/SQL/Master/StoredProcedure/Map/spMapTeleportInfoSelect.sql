/*************************************************************************************
    name        : (sp)[spMapTeleportInfoSelect]
    description : 맵 이동 정보
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성, 컬럼 추가





*************************************************************************************/
CREATE PROCEDURE [dbo].[spMapTeleportInfoSelect]
AS

BEGIN
    SELECT 
          [MapTeleportID]
        , [MapID]
        , [TeleportType]
        , [TeleportCostType]
        , [TeleportCostItemID]
        , [TeleportCostValue]
        , [PosX]
        , [PosY]
        , [PosZ]
      FROM
         MapTeleportInfo
END