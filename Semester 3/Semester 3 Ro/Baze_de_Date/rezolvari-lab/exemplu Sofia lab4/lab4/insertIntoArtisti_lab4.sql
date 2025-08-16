USE Spotify
GO

CREATE PROCEDURE insertIntoArtisti
(@test INT)
AS 
	BEGIN
		DECLARE @i INT,@j INT;
		SET @j=1;
		SELECT @i=NoOfRows FROM TestTables WHERE TestID=@test;
		WHILE @j<=@i
			BEGIN
				DECLARE @AId INT;
				SELECT @AId=MAX(AId)+1 FROM Artisti;
				INSERT INTO Artisti(Nume,Gen,DataNasterii,NrAbonati)
				VALUES
				(CONCAT('Nume',@AId),CONCAT('Gen',@AId),DATEADD(day, @AId % 65530, 0),@AId*10)
				SET @j=@j+1
			END
	END