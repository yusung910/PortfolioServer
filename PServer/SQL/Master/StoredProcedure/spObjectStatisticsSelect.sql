/*************************************************************************************
    name        : (sp)[spPilgrimStatSelect]
    description : 플레이어 캐릭터에 사용될 능력치 정보
                  PilgrimStatSelect 테이블의 PK 값을
                  아이템, 칭호 목록이 저장된 테이블이 참조한다
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성





*************************************************************************************/

CREATE PROCEDURE [dbo].[spObjectStatisticsSelect]
AS
BEGIN
    SELECT 
          [ObjectStatID]
        , [Strength]
        , [Dexterity]
        , [Enduracne]
        , [WeaponMastery]
        , [HP]
        , [MaxHP]
        , [MP]
        , [MaxMP]
        , [MeleeMinDamage]
        , [MeleeMaxDamage]
        , [MeleeDefence]
      FROM
        ObjectStatistics
END