/*************************************************************************************
    name        : (sp)[spMapInfoSelect]
    description : 맵 정보
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성
    2025.03.04.         유성            컬럼명 추가





*************************************************************************************/
CREATE PROCEDURE [dbo].[spMapInfoSelect]

AS
BEGIN
    SELECT [MapID]
         , [ParentMapID]
         , [MapType]
         , [MapName]
         , [MapSize]
         , [IsActive]
         , [IsSafeZone]
         , [IsTeleportable] 
      FROM MapInfo
END
