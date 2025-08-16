USE Spotify
GO

CREATE PROCEDURE insertIntoTests
(@nrTables INT,@nrViews INT)
AS
BEGIN 
	DECLARE @i INT,@j INT;
	DECLARE @tableName VARCHAR(50);
	SET @i=1;
	DECLARE @nrInreg INT 
	WHILE @i<=@nrTables
		BEGIN
			SELECT @tableName=Name FROM Tables WHERE TableID=@i;	
			SET @j=1
			SET @nrInreg=10
			WHILE @j<=@nrViews
				BEGIN
					INSERT INTO Tests(Name)
					VALUES 
					(CONCAT(@tableName,'_V',@j,'_',@nrInreg))
					SET @j=@j+1
					SET @nrInreg=@nrInreg*10
				END
			SET @i=@i+1;
		END
END
					
