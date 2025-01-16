CREATE TABLE [dbo].[AccountPlatform]
(
    [PlatformID] INT NOT NULL PRIMARY KEY, 
    [AccountSeq] INT NOT NULL DEFAULT 0, 
    [PlatformType] INT NULL, 
    [BuildType] INT NULL, 
    [AppVersion] INT NULL, 
    [ClientType] INT NULL, 
    [CreateDate] DATETIME NOT NULL DEFAULT (getdate()), 
    [UpdateDate] DATETIME NOT NULL DEFAULT (getdate())
)

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'Account 기본키',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPlatform',
    @level2type = N'COLUMN',
    @level2name = N'AccountSeq'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'로그인 플랫폼 (ELogin 99: GuestLogin,  1 : Google 등등)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPlatform',
    @level2type = N'COLUMN',
    @level2name = N'PlatformType'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'클라 타입(EClinet -> 0: window, 1: Android 등등)',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPlatform',
    @level2type = N'COLUMN',
    @level2name = N'ClientType'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'클라 AppVersion',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPlatform',
    @level2type = N'COLUMN',
    @level2name = N'AppVersion'
GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'클라 빌드 유형',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'AccountPlatform',
    @level2type = N'COLUMN',
    @level2name = N'BuildType'