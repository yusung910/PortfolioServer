/*************************************************************************************
    name        : (sp)[spAbilityStatRatePerTrainingStatSelect]
    description : 훈련 스탯(힘,민,맷,숙)에 따른 능력치 상승률 조회
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성





*************************************************************************************/

CREATE PROCEDURE [dbo].[spAbilityStatRatePerTrainingStatSelect]
AS
BEGIN
    SELECT
          [StatNo]
        , [Name]
        , [Remark]
        , [Strength]
        , [Dexterity]
        , [Endurance]
        , [WeaponMastery]
      FROM 
        AbilityStatRatePerTrainingStat
END