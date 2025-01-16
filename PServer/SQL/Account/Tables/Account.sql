/*************************************************************************************
    name        : (Table)[Account]
    description : 계정 관리 테이블

    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.16.         유성            최초 생성





*************************************************************************************/

CREATE TABLE [dbo].[Account]
(
    [AccountSeq]                    [int] IDENTITY(1,1)     NOT NULL,
    [AccountUkey]                   [varchar](255)          NOT NULL,
    [AccountStatus]                 [int]                   NOT NULL DEFAULT 0,
    [AccountType]                   [int]                   NOT NULL DEFAULT 0,
    [ConnectLoginServerID]          [int]                   NOT NULL,
    [OTP]                           [varchar](8)            NOT NULL,
    [IPAddress32]                   [int]                   NOT NULL,
    
    [StopExpireDate] DATETIME NULL, 
    CONSTRAINT [PK_Account] PRIMARY KEY CLUSTERED
    (
        [AccountSeq] ASC
    ) WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
)
GO
--Default Constraint

GO

GO

GO

GO

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'계정 고유 번호',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'Account',
    @level2type = N'COLUMN',
    @level2name = N'AccountSeq'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'계정 고유 key(문자열)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'Account',
    @level2type = N'COLUMN',
    @level2name = N'AccountUkey'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'계정 상태(0 : 정상, 9:삭제 대기, 91: 삭제 등등)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'Account',
    @level2type = N'COLUMN',
    @level2name = N'AccountStatus'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'계정 유형(0 : 일반 유저, 1: GM, 2: QA 등등)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'Account',
    @level2type = N'COLUMN',
    @level2name = N'AccountType'
GO

GO

GO

GO

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'접속한 로그인서버 ID',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'Account',
    @level2type = N'COLUMN',
    @level2name = N'ConnectLoginServerID'
GO

GO
