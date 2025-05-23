/*
* 게임에서 사용되는 능력치를 관리하는 테이블
* 캐릭터, 아이템 몬스터에 사용되는 능력치들이 저장된다
* Strength, DEX 등의 스탯들이 저장되는 테이블
*/
CREATE TABLE [dbo].[AbilityComponents]
(
    [Seq] INT NOT NULL PRIMARY KEY IDENTITY, 
    [AbilityName] VARCHAR(50) NULL, 
    [Comment] VARCHAR(50) NULL
);