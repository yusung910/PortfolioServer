/*************************************************************************************
    name        : (sp)[spAwakenAValueSelect]
    description : 각성에 따른 A값 조회
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.03.04.         유성            최초 생성





*************************************************************************************/

CREATE PROCEDURE [dbo].[spAwakenAValueSelect]
AS
BEGIN
    SELECT
        AwakenSeq
      , AValue
    FROM
        AwakenAValue
END