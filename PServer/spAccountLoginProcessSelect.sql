USE [Account]
GO

--ANSI_NULLS :: column_name = NULL을 사용하는 select 문은 column_name에 null 값이 있을 때도 0 행을 반환
SET ANSI_NULLS ON 
GO

-- 쌍 따옴표(")를 식별자로써 인식
SET QUOTED_IDENTIFIER ON
GO
/*************************************************************************************
    name        : (sp)[spAccountLoginProcessSelect]
    description : 게임 게정 로그인 처리
    result      : 프로시저 결과 값
        0 -> Success
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.07.         유성            최초 생성





*************************************************************************************/



CREATE PROCEDURE [dbo].[spAccountLoginProcessSelect]

  @Result                       INT         OUTPUT,
  @AccountSeq                   INT         OUTPUT,
  @AccountType                  INT         OUTPUT,
  @AccountStatus                INT         OUTPUT,
  -- 마지막 접속 게임서버ID
  @LatestConnectGameServerID    INT         OUTPUT,     

  -- 접속된 로그인서버 ID
  @ConnectedLoginServerID       INT         OUTPUT,     
  @SessionDate                  DATETIME    OUTPUT,

  @LoginPlatformType            INT,
  @AccountUKey                  VARCHAR(256),

  -- (from server)접속 시도하는 로그인 서버ID
  @ConnectingLoginServerID      INT,                    

  @ClientType                   INT,
  @AppVersion                   INT


AS

BEGIN
    --Transact-SQL문 또는 저장 프로시저의 영향을 받은 행의 수를 나타내는 메시지가 결과 집합의 일부로 반환되지 않도록 한다.
    SET NOCOUNT ON;

    SET @Result = 0
    SET @AccountSeq = 0
    SET @AccountType = 0
    SET @LatestConnectGameServerID = 0
    SET @SessionDate = '1900-01-01'

    --계정 유형
    DECLARE @Account_Type               INT = 99

    --캐릭터가 있는 서버 목록
    DECLARE @ExsistCharacterServerIDs   TABLE
    (
        ServerID        INT,
        CharacterLevel  INT,
        LastTime        DATETIME
    )

    --계정 상태
    DECLARE @Account_Status             INT = 0

    BEGIN
        SELECT
            @AccountSeq = AccountSeq
            , @AccountType = AccountType
        FROM
            Account
        WHERE
            AccountUKey = @AccountUKey
            AND LoginPlatformType = @LoginPlatformType
    END


    IF(0 = @AccountSeq)
        BEGIN
            --계정 생성
            BEGIN TRAN
                -- 기본 계정
                SET @AccountType = @Account_Type

                -- 1. Account Table
                INSERT INTO Account(AccountUKey, LoginPlatformType, ClientType, ConnectLoginServerID, OTP)
                VALUES (@AccountUKey, @LoginPlatformType, @ClientType, @ConnectingLoginServerID, @OTP)

                -- Check
                IF(1 <> @@ROWCOUNT)
                BEGIN
                    ROLLBACK TRAN
                    SET @Result = -1
                    RETURN
                END

                SET @AccountSeq = @@IDENTITY
            COMMIT TRAN

            SET @Result = 0
        END

    ELSE
        BEGIN
            --기존 계정
            SELECT
                @ConnectedLoginServerID = ConnectLoginServerID,
                @AccountStatus = AccountStatus
              FROM 
                Account
             WHERE
                AccountSeq = @AccountSeq
            
            -- 기존 계정의 상태 여부에 따른 result값 출력
            -- EAccountStatus
            -- 계정의 상태가 정상이 아닐경우 
            IF(@AccountStatus > 1)
            BEGIN
                UPDATE
                        Account
                    SET
                        ConnectLoginServerID = 0,
                        OTP = NULL
                  WHERE
                        AccountSeq = @AccountSeq
                IF(@AccountStatus = 4)
                    SET @Result = 4     --계정 영구 정지
                
                ELSE IF(@AccountStatus = 5)
                    SET @Result = 5     --계정 기간 정지


                


                RETURN
            END
        END
END
 