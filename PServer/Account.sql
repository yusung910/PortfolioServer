USE [Account]

CREATE TABLE [dbo].[Account]
(
    --계정 고유 번호
    [AccountSeq]                [int] IDENTITY(1,1)     NOT NULL,
    --계정 고유 key(문자열)
    [AccountUIDkey]             [varchar](255)          NOT NULL,
    --계정 상태(0 : 정상, 9:삭제 대기, 91: 삭제)
    [AccountStatus]             [int]                   NOT NULL,
    --계정 유형(0 : 일반 유저, 1: GM, 2: QA 등등)
    [AccountType]               [int]                   NOT NULL,
    --마지막 접속 서버 ID
    [LatestConnectServerID]     [int]                   NULL,
    --otp
    [OTP]                       [varchar](8)            NULL,
    -- 생성일
    [CreateDate]                [datetime]              NULL,
    -- 수정일
    [UpdateDate]                [datetime]              NULL,
    
    CONSTRAINT [PK_Account] PRIMARY KEY CLUSTERED
    (
        [AccountSeq] ASC
    ) WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
)


--Default Constraint
ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_OTP] DEFAULT ((0)) FOR [OTP]
ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_LatestConnectServerID] DEFAULT ((0)) FOR [LatestConnectServerID]

ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_CreateDate] DEFAULT (getdate()) FOR [CreateDate]

ALTER TABLE [dbo].[Account] ADD CONSTRAINT [DF_Account_UpdateDate] DEFAULT (getdate()) FOR [UpdateDate]