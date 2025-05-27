CREATE TABLE [dbo].[MapTeleportInfo]
(
    [MapTeleportID] INT NOT NULL PRIMARY KEY, 
    [MapID] INT NOT NULL, 
    [TeleportType] INT NOT NULL, 
    [TeleportCostType] INT NOT NULL DEFAULT 0, 
    [TeleportCostItemID] INT NOT NULL DEFAULT 0,
    [TeleportCostValue] INT NOT NULL DEFAULT 0,
    [PosX] FLOAT NOT NULL DEFAULT 0, 
    [PosY] FLOAT NOT NULL DEFAULT 0, 
    [PosZ] FLOAT NOT NULL DEFAULT 0
)

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'기본키',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapTeleportInfo',
    @level2type = N'COLUMN',
    @level2name = N'MapTeleportID'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'MapInfo 기본키',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapTeleportInfo',
    @level2type = N'COLUMN',
    @level2name = 'MapID'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'텔레포트 유형(Enum)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapTeleportInfo',
    @level2type = N'COLUMN',
    @level2name = N'TeleportType'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'텔레포트 비용',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapTeleportInfo',
    @level2type = N'COLUMN',
    @level2name = 'TeleportCostValue'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'텔레포트 비용 유형(Enum)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapTeleportInfo',
    @level2type = N'COLUMN',
    @level2name = N'TeleportCostType'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'텔레포트 소모 아이템',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'MapTeleportInfo',
    @level2type = N'COLUMN',
    @level2name = N'TeleportCostItemID'