/*************************************************************************************
    name        : (sp)[spAccountCharacterListSelect]
    description : 계정 별 캐릭터 조회
    result      : 프로시저 결과 값(EDBResult)
        0 -> Success 
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.15.         유성            최초 생성





*************************************************************************************/

CREATE PROCEDURE [dbo].[spAccountCharacterListSelect]
    @AccountSeq         INT

AS
SET NOCOUNT ON
SET LOCK_TIMEOUT 3000
SET XACT_ABORT ON
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
BEGIN
    SELECT CharacterSeq
         , ServerID
         , CharacterName
         , CharacterLevel
         , CharacterClass
         , LastTime
     FROM AccountCharacter
    WHERE AccountSeq = @AccountSeq
      --캐릭터 상태 값 (Enum)에 따라 유효한 상태 값만 넣는다
      --default 값 : 0
      AND [State] IN (0) 
END
