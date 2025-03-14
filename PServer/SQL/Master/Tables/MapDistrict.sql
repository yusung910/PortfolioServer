CREATE TABLE [dbo].[MapDistrict]
(
    [MapDistrictD] INT NOT NULL PRIMARY KEY, 
    [MapID] INT NOT NULL, 
    [TransportNPCID] INT NOT NULL,
    [MapDistrictGroup] INT NOT NULL, 
    [MapDistrictType] INT NOT NULL, 
    [MapDistrictName] VARCHAR(50) NOT NULL, 
    --해당 지역에 텔레포트로 진입 할 수 있는 여부
    [TeleportEnterYN] BIT NOT NULL, 
    --해당 지역에서 텔레포트로 나갈 수 있는지 여부
    [TeleportEscapeYN] BIT NOT NULL
)