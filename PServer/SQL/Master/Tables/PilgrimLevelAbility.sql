﻿--캐릭터(Pilgrim) 레벨 별 기본 스탯
CREATE TABLE [dbo].[PilgrimLevelAbility]
(
    [TrainingAbilityType] INT NOT NULL, 
    [Level] INT NOT NULL , 
    [AbilityModuleSeq] INT NOT NULL, 
    [NeedExp] BIGINT NOT NULL, 
    CONSTRAINT [PK_AbilityModulePilgrimLevel] PRIMARY KEY ([Level], [TrainingAbilityType], [AbilityModuleSeq]) 
)
