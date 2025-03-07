CREATE TABLE [dbo].[NPC]
(
    [Sequence] INT NOT NULL PRIMARY KEY, 
    [Name] VARCHAR(50) NOT NULL, 
    [Type] INT NOT NULL DEFAULT 0,
    [MonsterGrade] INT NOT NULL DEFAULT 0, 
    [MonsterGroup] INT NOT NULL DEFAULT 0, 
    [SupporterGroup] INT NOT NULL DEFAULT 0 
)
