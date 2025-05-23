-- 게임 몬스터, 아이템, 플레이어 적용될 능력치 정보
-- 아이템, 칭호 등등에 사용되는 값들이 저장되는 테이블
CREATE TABLE [dbo].[AbilityModule]
(
    [Seq] INT NOT NULL , 
    [AbilityComponentsSeq] INT NOT NULL, 
    [Value] INT NOT NULL DEFAULT 0, 
    [Remark] VARCHAR(50) NULL, 
    PRIMARY KEY ([AbilityComponentsSeq], [Seq]), 
    CONSTRAINT [FK_AbilityModule_AbilityComponents] FOREIGN KEY ([AbilityComponentsSeq]) REFERENCES [AbilityComponents](Seq) 
);