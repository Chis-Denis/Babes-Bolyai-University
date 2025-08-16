USE Spotify
GO

CREATE PROCEDURE insertIntoColaborari
(@noOfRows INT)
AS 
	BEGIN
		DECLARE @i INT,@j INT;
		SET @j=1;
		SET @i=@NoOfRows;
		WHILE @j<=@i
			BEGIN
				DECLARE @ColabId INT;
				SELECT @ColabId=MAX(ColabId)+1 FROM Colaborari;
				INSERT INTO  Colaborari(ColabId,Denumire,NrAscultari)
				VALUES
				(@ColabId,CONCAT('Denumire',@ColabId),@ColabId*10)
				SET @j=@j+1
			END
	END