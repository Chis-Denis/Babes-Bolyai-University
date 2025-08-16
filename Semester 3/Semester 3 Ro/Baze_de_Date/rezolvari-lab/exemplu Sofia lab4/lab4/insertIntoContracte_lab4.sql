USE Spotify
GO

CREATE PROCEDURE insertIntoContracte
AS
	BEGIN

	DECLARE @Table2 TABLE(AId INT)
	INSERT INTO @Table2(AId)
	SELECT  AId FROM Artisti
	--SELECT * FROM @Table2

	DECLARE @Table3 TABLE(	ColabId INT)
	INSERT INTO @Table3(ColabId)
	SELECT  ColabId FROM Colaborari
	--SELECT * FROM @Table3

	DECLARE @Table TABLE(TId INT IDENTITY,AId INT,ColabId INT,Onorariu MONEY,DrepturiAutor VARCHAR(100))
	INSERT INTO @Table(AId,ColabId)
	SELECT AId,ColabId
	FROM @Table2
	CROSS JOIN 
	@Table3

	DECLARE @nr INT;
	SELECT @nr=MAX(TId) FROM @Table
	DECLARE @i INT;
	SET @i=1;
	WHILE @i<=@nr
		BEGIN
		UPDATE @Table
		SET Onorariu=@i*10,DrepturiAutor=CONCAT('Drepturi de autor',@i)
		WHERE TId=@i
		SET @i=@i+1
		END

	INSERT INTO Contracte
	SELECT AId,ColabId,Onorariu,DrepturiAutor FROM @Table

	END



		