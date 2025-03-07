-- 캐릭터(pilgrim)에 적용될 능력치 정보
-- 아이템, 칭호 등등에 사용되는 정보들이 저장되는 테이블
CREATE TABLE [dbo].[PilgrimStat]
(
    [Sequence] INT NOT NULL PRIMARY KEY, 
    [Remark] NVARCHAR(50) NULL,
    [Strength] INT NOT NULL, 
    [Dexterity] INT NOT NULL, 
    [Enduracne] INT NOT NULL, 
    [WeaponMastery] INT NOT NULL, 
    [HP] INT NOT NULL, 
    [MaxHP] INT NOT NULL, 
    [MP] INT NOT NULL, 
    [MaxMP] INT NOT NULL, 
    [MeleeMinDamage] INT NOT NULL, 
    [MeleeMaxDamage] INT NOT NULL, 
    [MeleeDefence] INT NOT NULL
)