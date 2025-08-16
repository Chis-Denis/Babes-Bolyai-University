USE Spotify
GO

CREATE PROCEDURE execTest
(@TestName VARCHAR(50))
AS
	BEGIN
	DECLARE @TestId INT
	SELECT @TestId =TestID FROM Tests WHERE Name=@TestName
	EXEC @TestName @TestId
END