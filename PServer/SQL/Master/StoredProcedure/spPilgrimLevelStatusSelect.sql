/*************************************************************************************
    name        : (sp)[spPilgrimLevelStatus]
    description : Player Character(Pilgrim)이 훈련하는 스텟의 경험치 정보
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성
    2025.05.23.         유성            구조 변경에 따른 수정





*************************************************************************************/

CREATE PROCEDURE [dbo].[spPilgrimLevelStatus]
AS
BEGIN
    SELECT
        [TrainingAbilityType]
      , [Level]
      , [AbilityModuleSeq]
      , [NeedNextLevelExp]
      FROM 
        PilgrimLevelStatus
END