/*************************************************************************************
    name        : (sp)[spAbilityModulePilgrimLevelSelect]
    description : 레벨에 따른 기본 능력치
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성
    2025.05.23.         유성            구조 변경에 따른 수정





*************************************************************************************/
CREATE PROCEDURE [dbo].[spAbilityModulePilgrimLevelSelect]
AS
BEGIN
    SELECT  [Level]
          , [AbilityModuleSeq]
      FROM
        AbilityModulePilgrimLevel
END