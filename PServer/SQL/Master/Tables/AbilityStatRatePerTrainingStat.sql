--캐릭터의 훈련 능력치(TrainingStat :: 힘, 민, 맷, 숙)에 따른
--캐릭터의 능력치(AbilityStat) 반영 비율
CREATE TABLE [dbo].[AbilityStatRatePerTrainingStat]
(
    [StatNo] INT NOT NULL PRIMARY KEY, 
    [Name] NVARCHAR(50) NOT NULL, 
    [Remark] NVARCHAR(50) NOT NULL, 
    [Strength] INT NOT NULL, 
    [Dexterity] INT NOT NULL, 
    [Endurance] INT NOT NULL, 
    [WeaponMastery] INT NOT NULL
)
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'시퀀스',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStatRatePerTrainingStat',
    @level2type = N'COLUMN',
    @level2name = N'StatNo'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'힘',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStatRatePerTrainingStat',
    @level2type = N'COLUMN',
    @level2name = N'Strength'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'민첩',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStatRatePerTrainingStat',
    @level2type = N'COLUMN',
    @level2name = N'Dexterity'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'맷집',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStatRatePerTrainingStat',
    @level2type = N'COLUMN',
    @level2name = N'Endurance'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'무기숙련도',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AbilityStatRatePerTrainingStat',
    @level2type = N'COLUMN',
    @level2name = N'WeaponMastery'