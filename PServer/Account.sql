USE [Account]
GO

--ANSI_NULLS :: column_name = NULL을 사용하는 select 문은 column_name에 null 값이 있을 때도 0 행을 반환
SET ANSI_NULLS ON 
GO

-- 쌍 따옴표(")를 식별자로써 인식
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Account]
(
    --계정 고유 번호
    [AccountSeq]                    [int] IDENTITY(1,1)     NOT NULL,
    --계정 고유 key(문자열)
    [AccountUkey]                   [varchar](255)          NOT NULL,
    --계정 상태(0 : 정상, 9:삭제 대기, 91: 삭제 등등)
    [AccountStatus]                 [int]                   NOT NULL,
    --계정 유형(0 : 일반 유저, 1: GM, 2: QA 등등)
    [AccountType]                   [int]                   NOT NULL,
    --로그인 플랫폼 (ELogin 99: GuestLogin,  1 : Google 등등)
    [LoginPlatformType]             [int]                   NOT NULL,
    --클라 타입(EClinet -> 0: window, 1: Android 등등)
    [ClientType]                    [int]                   NOT NULL,
    -- 접속한 로그인서버 ID
    [ConnectLoginServerID]          [int]                   NULL,
    --otp
    [OTP]                           [varchar](8)            NULL,
    -- 생성일
    [CreateDate]                    [datetime]              NULL,
    -- 수정일
    [UpdateDate]                    [datetime]              NULL,
    
    CONSTRAINT [PK_Account] PRIMARY KEY CLUSTERED
    (
        [AccountSeq] ASC
    ) WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
)


--Default Constraint
ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_OTP] DEFAULT ((0)) FOR [OTP]
ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_AccountStatus] DEFAULT ((0)) FOR [AccountStatus]

ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_LatestConnectServerID] DEFAULT ((0)) FOR [LatestConnectServerID]

ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_CreateDate] DEFAULT (getdate()) FOR [CreateDate]

ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_UpdateDate] DEFAULT (getdate()) FOR [UpdateDate]