/*************************************************************************************
    name        : (sp)[spLevelBaseAbilityStatSelect]
    description : 레벨에 따른 기본 능력치
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성





*************************************************************************************/
CREATE PROCEDURE [dbo].[spLevelBaseAbilityStatSelect]
AS
BEGIN
    SELECT
          [Level]
        , [MaxHP]
        , [MaxMana]
        , [HPRegen]
        , [ManaRegen]
        , [Defence]
        , [MeleeDamageMin]
        , [MeleeDamageMax]
      FROM
        LevelBaseAbilityStat
END