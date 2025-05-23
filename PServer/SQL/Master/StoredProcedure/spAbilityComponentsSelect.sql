/*************************************************************************************
    name        : (sp)[spAbilityComponentsSelect]
    description : 캐릭터 능력치, 아이템에 적용될 속성들을 호출한다.
    result      : 프로시저 결과 값(MDBAbilityComponents)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성
    2025.05.23.         유성            테이블 변경에 따른 수정




*************************************************************************************/

CREATE PROCEDURE [dbo].[spAbilityComponentsSelect]
AS
BEGIN
    SELECT 
            [Seq]
      FROM
        AbilityComponents
END