CREATE TABLE [dbo].[NPCSummons]
(
    [MapID] INT NOT NULL , 
    [NPCUniqueID] INT NOT NULL, 
    [SummonName] VARCHAR(50) NOT NULL, 
    [SummonGroup] INT NOT NULL, 
    [ResummonTime] INT NOT NULL, 
    [Rotation] INT NOT NULL,
    PRIMARY KEY ([MapID], [NPCUniqueID])
)
