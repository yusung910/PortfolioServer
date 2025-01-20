/*************************************************************************************
    name        : (sp)[spAccountConnectServerIDClear]
    description : 게임 계정 접속 서버 정보 수정
    result      : 프로시저 결과 값(EDBResult)
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.20.         유성            최초 생성





*************************************************************************************/
CREATE PROCEDURE [dbo].[spAccountConnectServerIDClear]
    @AccountSeq                     INT,
    @ConnectServerID                INT
AS
BEGIN
	SET NOCOUNT ON
	SET LOCK_TIMEOUT 3000
	SET XACT_ABORT ON
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED

    UPDATE Account
       SET ConnectServerID = 0
     WHERE AccountSeq = @AccountSeq

END


