--캐릭터(Pilgrim) 레벨 별 기본 스탯
CREATE TABLE [dbo].[PilgrimLevelBaseStat]
(
    [Level] INT NOT NULL PRIMARY KEY, 
    [MaxHP] INT NOT NULL, 
    [MaxMana] INT NOT NULL, 
    [HPRegen] INT NOT NULL, 
    [ManaRegen] INT NOT NULL, 
    [Defence] INT NOT NULL, 
    [MeleeDamageMin] INT NOT NULL, 
    [MeleeDamageMax] INT NOT NULL
)
