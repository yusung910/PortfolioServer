/*************************************************************************************
    name        : (sp)[spAccountLoginProcessSelect]
    description : 게임 계정 생성 및 조회 처리
    result      : 프로시저 결과 값(EDBResult)
        0 -> Success 
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.07.         유성            최초 생성





*************************************************************************************/
CREATE PROCEDURE [dbo].[spAccountLoginProcessSelect]

    @Result                       INT         OUTPUT
  , @AccountSeq                   INT         OUTPUT
  , @AccountType                  INT         OUTPUT
  --계정상태
  , @AccountStatus                INT         OUTPUT
  -- 마지막 접속 서버ID
  , @LatestConnectServerID        INT         OUTPUT     

  -- 접속된 서버 ID
  , @ConnectedServerID            INT         OUTPUT
  -- 삭제 대기기간
  , @RemainingPeriod              DATETIME    OUTPUT

  --계정 로그인 플랫폼 유형
  , @LoginPlatformType            INT
  , @AccountUKey                  VARCHAR(255)
  , @PlatformID                   VARCHAR(255)

  -- (from server)접속 시도하는 로그인 서버ID
  , @ConnectingServerID           INT
  , @OTP                          INT
  , @ClientType                   INT
  , @AppVersion                   INT
  , @BuildType                    INT
  , @IPAddress32                  INT

AS

BEGIN
    --Transact-SQL문 또는 저장 프로시저의 영향을 받은 행의 수를 나타내는 메시지가 결과 집합의 일부로 반환되지 않도록 한다.
	SET NOCOUNT ON
	SET LOCK_TIMEOUT 3000
	SET XACT_ABORT ON
	SET TRANSACTION ISOLATION LEVEL READ COMMITTED

    SET @Result = 0
    SET @AccountSeq = 0
    SET @AccountType = 0
    SET @AccountStatus = 0
    SET @LatestConnectServerID = 0
    SET @ConnectedServerID = 0
    SET @RemainingPeriod = '1900-01-01'

    --계정 유형
    DECLARE @Account_Type               INT = 99

    --캐릭터가 있는 서버 목록
    DECLARE @ExsistPilgrimServerID    TABLE
    (
          ServerID          INT
        , PilgrimLevel      INT
        , LastTime          DATETIME
    )

    
    DECLARE @Platform_GuestLogin        INT = 99
    BEGIN
        SELECT
              @AccountSeq = AccountSeq
            , @AccountType = AccountType
        FROM
            Account
        WHERE
            AccountUKey = @AccountUKey
    END


    IF(0 = @AccountSeq)
        BEGIN
            --계정 생성
            BEGIN TRAN
                -- 기본 계정
                SET @AccountType = @Account_Type

                -- 1. Account Table
                INSERT INTO Account(AccountUKey, ConnectServerID, OTP, IPAddress32)
                VALUES (@AccountUKey, @ConnectingServerID, @OTP, @IPAddress32)

                -- Check
                IF(1 <> @@ROWCOUNT)
                BEGIN
                    ROLLBACK TRAN
                    SET @Result = -1
                    RETURN
                END

                -- @@IDENTITY : 현재 세션의 테이블에서 생성(insert)된 마지막 ID 값을 반환
                SET @AccountSeq = @@IDENTITY

                -- 다른 플랫폼(NAVER, GOOGLE, FACEBOOK 등 로그인 할 경우)
                IF(@Platform_GuestLogin <> @LoginPlatformType)
                    BEGIN
                        --AccountPlatform 테이블
                        INSERT INTO AccountPlatform(PlatformID, AccountSeq, LoginPlatformType, BuildType, AppVersion, ClientType)
                        VALUES(@PlatformID, @AccountSeq, @LoginPlatformType, @BuildType, @AppVersion, @ClientType)

                        IF(1 <> @@ROWCOUNT)
                        BEGIN
                            ROLLBACK TRAN
                            SET @Result = 1
                            RETURN
                        END
                    END

            COMMIT TRAN

            SET @Result = 0
        END
    ELSE
        BEGIN
            --기존 계정
            SELECT
                  @ConnectedServerID = ConnectServerID
                , @AccountStatus = AccountStatus
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
                        ConnectServerID = 0,
                        OTP = NULL
                  WHERE
                        AccountSeq = @AccountSeq

                IF(@AccountStatus = 3)
                    SET @Result = 3     --계정 영구 정지
                
                ELSE IF(@AccountStatus = 4)
                    SET @Result = 4     --계정 일시 정지

                --특정 상태가 더 있을 경우 추가
                RETURN
            END

            BEGIN
                DECLARE @Table TABLE
                (
                      AccountSeq                  INT
                    , AccountType                 INT
                    , LastLoginServer             INT
                )

                UPDATE 
                    Account
                SET
                      ConnectServerID = @ConnectingServerID
                    , OTP = @OTP
                    , IPAddress32 = @IPAddress32
                OUTPUT
                      inserted.AccountSeq,
                      inserted.AccountType,
                      inserted.ConnectServerID
                INTO @Table

                WHERE
                    AccountSeq = @AccountSeq

                IF(1 <> @@ROWCOUNT)
                BEGIN
                    SET @Result = -2
                    SET @AccountType =  -1
                END

                SELECT 
                      @AccountSeq = AccountSeq
                    , @AccountType = AccountType
                    , @LatestConnectServerID = LastLoginServer
                FROM
                    @Table
            END

            --이미 접속 되어 있는지 여부 확인
            IF(0 <> @ConnectedServerID)
                SET @Result = 1
            ELSE
                SET @Result = 0

            INSERT INTO
                @ExsistPilgrimServerID ( ServerID, PilgrimLevel, LastTime)
            SELECT
                ServerID, PilgrimLevel, LastTime
            FROM
            (
                SELECT
                    ServerID, PilgrimLevel, LastTime,
                    ROW_NUMBER() OVER(PARTITION BY ServerID ORDER BY PilgrimLevel DESC, LastTime DESC) AS LevelRank
                FROM
                    AccountPilgrim
                WHERE
                    AccountSeq = @AccountSeq
                    AND [State] IN ( 1, 2, 12)
            ) AS A
            WHERE
                LevelRank = 1
        END

        SELECT ServerID
        FROM @ExsistPilgrimServerID
END
 