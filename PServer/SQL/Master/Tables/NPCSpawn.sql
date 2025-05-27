CREATE TABLE [dbo].[NPCSpawn]
(
    [MapID] INT NOT NULL , 
    [NPCUniqueID] INT NOT NULL, 
    [SpawnName] VARCHAR(50) NOT NULL, 
    [SpawnGroup] INT NOT NULL, 
    [RespawnTime] INT NOT NULL, 
    [Rotation] INT NOT NULL,
    PRIMARY KEY ([MapID], [NPCUniqueID])
)
