-- 캐릭터(pilgrim)에 적용될 능력치 정보
-- 아이템, 칭호 등등에 사용되는 정보들이 저장되는 테이블
CREATE TABLE [dbo].[PilgrimStat]
(
    [Sequence] INT NOT NULL PRIMARY KEY, 
    [Strength] INT NOT NULL, 
    [Dexterity] INT NOT NULL, 
    [Enduracne] INT NOT NULL, 
    [WeaponMastery] INT NOT NULL, 
    [HP] INT NOT NULL, 
    [MaxHP] INT NOT NULL, 
    [MP] INT NOT NULL, 
    [MaxMP] INT NOT NULL, 
    [MeleeMinDamage] INT NULL, 
    [MeleeMaxDamage] INT NULL, 
    [MeleeDefence] INT NULL
)

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'기본키',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'Sequence'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'힘',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'Strength'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'민첩',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'Dexterity'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'맷집',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'Enduracne'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'무기숙련도',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'WeaponMastery'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'체력',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'HP'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'최대생명력',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'MaxHP'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'내공',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'MP'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'최대내공',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'HeroStat',
    @level2type = N'COLUMN',
    @level2name = N'MaxMP'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'근접 최소 데미지',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStat',
    @level2type = N'COLUMN',
    @level2name = N'MeleeMinDamage'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'근접 최대 데미지',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStat',
    @level2type = N'COLUMN',
    @level2name = N'MeleeMaxDamage'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'근접 방어력',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStat',
    @level2type = N'COLUMN',
    @level2name = N'MeleeDefence'