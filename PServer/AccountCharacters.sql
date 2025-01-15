USE [Account]
GO

--ANSI_NULLS :: column_name = NULL을 사용하는 select 문은 column_name에 null 값이 있을 때도 0 행을 반환
SET ANSI_NULLS ON 
GO

-- 쌍 따옴표(")를 식별자로써 인식
SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[AccountCharacter]
(
    -- 캐릭터 고유 번호
    [CharacterSeq]              [int] IDENTITY(1,1)     NOT NULL,

    -- 계정 고유 번호
    [AccountSeq]                [int]                   NOT NULL,

    -- 서버ID
    [ServerID]                  [int]                   NOT NULL,

    -- 캐릭터 명
    [CharacterName]             [nvarchar](100)         NOT NULL,

    -- 레벨
    [CharacterLevel]            [int]                   NOT NULL,
    
    -- 클래스
    [CharacterClass]            [int]                   NOT NULL,

    -- 상태
    [State]                     [int]                   NOT NULL,

    -- 최종 접속일
    [LastTime]                  [datetime]              NOT NULL,

    --기본키 제약조건 설정
    CONSTRAINT [PK_AccountCharacters] PRIMARY KEY CLUSTERED
    (
        [CharacterSeq] ASC
    ) WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY],

    --AccountSeq 외래키 제약조건 설정
    CONSTRAINT [FK_AccountSeq_AccountTbl] FOREIGN KEY (AccountSeq) REFERENCES Account([AccountSeq])
)

--DEFAULT CONSTRAINT  
GO
ALTER TABLE [dbo].[AccountCharacters] ADD CONSTRAINT [DF_Character_State] DEFAULT ((0)) FOR [State]

