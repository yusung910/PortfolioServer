/*************************************************************************************
    name        : (sp)[spServerConstSelect]
    description : Account 설정 데이터 호출
    result      : 프로시저 결과 값(EDBResult)
        0 -> Success 
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.25.         유성            최초 생성





*************************************************************************************/

CREATE PROCEDURE [dbo].[spServerConstSelect]

AS
BEGIN
    SET NOCOUNT ON;

    SELECT seq, [Value]
      FROM ServerConst
END
