/*************************************************************************************
    name        : (Table)[AccountPilgrim]
    description : 계정 별 캐릭터 관리 테이블

    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.01.16.         유성            최초 생성





*************************************************************************************/


CREATE TABLE [dbo].[AccountPilgrim]
(
    [PilgrimSeq]                [int] IDENTITY(1,1)     NOT NULL,
    [AccountSeq]                [int]                   NOT NULL,
    [ServerID]                  [int]                   NOT NULL,
    [PilgrimName]               [nvarchar](100)         NOT NULL,
    [PilgrimLevel]              [int]                   NOT NULL DEFAULT 0,
    [PilgrimClass]              [int]                   NOT NULL DEFAULT 0,
    [State]                     [int]                   NOT NULL DEFAULT 0,
    [LastTime]                  [datetime]              NOT NULL,

    CONSTRAINT [PK_AccountPilgrimSeq] PRIMARY KEY CLUSTERED
    (
        [PilgrimSeq] ASC
    ) WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY],

    --AccountSeq 외래키 제약조건 설정
    CONSTRAINT [FK_AccountSeq_AccountTbl] FOREIGN KEY (AccountSeq) REFERENCES Account([AccountSeq])
)

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'캐릭터 고유 번호',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'PilgrimSeq'



GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'마지막 접속 시간',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'LastTime'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'캐릭터 상태',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'State'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'캐릭터 직업',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'PilgrimClass'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'캐릭터 레벨',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'PilgrimLevel'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'캐릭터 명',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'PilgrimName'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'서버 ID',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'ServerID'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'계정 고유 번호',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPilgrim',
    @level2type = N'COLUMN',
    @level2name = N'AccountSeq'
