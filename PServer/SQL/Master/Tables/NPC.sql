CREATE TABLE [dbo].[NPC]
(
    [NPCUniqueID] INT NOT NULL , 
    [AttributeModuleSeq] INT NOT NULL,
    [Name] VARCHAR(50) NOT NULL, 
    [NPCType] INT NOT NULL DEFAULT 0,
    [NPCGrade] INT NOT NULL DEFAULT 0, 
    [NPCGroup] INT NOT NULL DEFAULT 0, 
    [NPCRace] INT NOT NULL DEFAULT 0, --종족
    [Renown] INT NOT NULL, 
    [Exp] INT NOT NULL, 
    PRIMARY KEY ([AttributeModuleSeq], [NPCUniqueID])
)
