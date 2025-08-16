USE Spotify
GO

CREATE PROCEDURE insertIntoCantece
(@test INT)
AS 
	BEGIN
		DECLARE @i INT,@j INT;
		SET @j=1;
		SELECT @i=NoOfRows FROM TestTables WHERE TestID=@test;
		WHILE @j<=@i
			BEGIN
				DECLARE @CtcId INT;
				SELECT @CtcId=MAX(CtcId)+1 FROM Cantece;
				DECLARE @AId INT;
				SELECT @AId=MAX(AId) FROM Artisti;
				DECLARE @Albd INT;
				SELECT @Albd=MAX(AlbId) FROM Albume;
				DECLARE @PId INT;
				SELECT @PId=MAX(PId) FROM Playlisturi;
				INSERT INTO Cantece(CtcId,Denumire,Durata,AlbId,Aid,PId)
				VALUES
				(@CtcId,CONCAT('Denumire',@CtcId),1+@CtcId,@Albd,@AId,@PId)
				SET @j=@j+1
			END
	END