CREATE TABLE [dbo].[NPC]
(
    [NPCUniqueID] INT NOT NULL PRIMARY KEY, 
    [ObjectStatID] INT NOT NULL,
    [Name] VARCHAR(50) NOT NULL, 
    [NPCType] INT NOT NULL DEFAULT 0,
    [MonsterGrade] INT NOT NULL DEFAULT 0, 
    [MonsterGroup] INT NOT NULL DEFAULT 0, 
    [SupporterGroup] INT NOT NULL DEFAULT 0, 
    [Level] INT NOT NULL, 
    [Exp] INT NOT NULL
)
