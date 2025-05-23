/*************************************************************************************
    name        : (sp)[spObjectAbilityGroupSelect]
    description : 
    result      : 
        
    Date                Author          Description
    --------------      ---------       ---------------------------------------------
    2025.05.23.         유성            최초생성



*************************************************************************************/

CREATE PROCEDURE [dbo].[spAbilityModuleSelect]
AS
BEGIN
    SELECT [Seq]
         , [AbilityComponentsSeq]
         , [Value]
      FROM AbilityModule
END
