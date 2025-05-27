CREATE TABLE [dbo].[NPCSpawnGroup]
(
    [NPCSpawnGroupSeq] INT NOT NULL , 
    [NPCUniqueID] INT NOT NULL, 
    [SpawnProbability] INT NOT NULL DEFAULT 0, 
    PRIMARY KEY ([NPCSpawnGroupSeq], [NPCUniqueID])
)
