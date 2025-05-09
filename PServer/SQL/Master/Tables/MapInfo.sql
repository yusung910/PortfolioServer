CREATE TABLE [dbo].[MapInfo]
(
    [MapID] INT NOT NULL PRIMARY KEY, 
    [ParentMapID] INT NOT NULL, 
    [MapType] INT NOT NULL,
    [MapFileName] NVARCHAR(256) NOT NULL, 
    [MapFilePath] NVARCHAR(256) NOT NULL,
    [MapSize] INT NOT NULL,
    [ActiveYN] TINYINT NOT NULL, 
    [SafeZoneYN] TINYINT NOT NULL, 
    [TeleportableYN] TINYINT NOT NULL
)

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'기본키',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapInfo',
    @level2type = N'COLUMN',
    @level2name = N'MapID'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'안전지대여부',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapInfo',
    @level2type = N'COLUMN',
    @level2name = 'SafeZoneYN'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'부모MapID',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapInfo',
    @level2type = N'COLUMN',
    @level2name = N'ParentMapID'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'맵 유형(Enum Map)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapInfo',
    @level2type = N'COLUMN',
    @level2name = N'MapType'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'텔레포트 가능 여부',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapInfo',
    @level2type = N'COLUMN',
    @level2name = 'TeleportableYN'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'맵 파일 경로',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapInfo',
    @level2type = N'COLUMN',
    @level2name = N'MapFilePath'