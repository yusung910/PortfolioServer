USE [Account]

/*************************************************************************************
    name        : (sp)[spAccountLoginProcessSelect]
    description : 게임 게정 로그인 처리
    result      : 



    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.07.         유성            최초 생성





*************************************************************************************/



ALTER PROCEDURE [dbo].[spAccountLoginProcessSelect]
  @Result                   INT         OUTPUT,
  @AccountSeq               INT         OUTPUT,
  @AccountType              INT         OUTPUT,
  @LatestConnectServerID    INT         OUTPUT,
  @ConnectServerID          INT         OUTPUT,     -- 현재 접속 서버 ID
  @SessionDate              DATETIME    OUTPUT,

  @LoginType                INT,
  @AccountUIDKey            VARCHAR(256),
  @ConnectLoginServerID     INT,

  @ClientType               INT,
  @AppVersion               INT



 