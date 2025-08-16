USE Spotify
GO

CREATE PROCEDURE insertIntoViews
(@nrViews INT)
AS 
BEGIN
	DECLARE @j INT;
	SET @j=1;
	WHILE @j<=@nrViews
		BEGIN
		INSERT INTO Views(Name)
		VALUES
		(CONCAT('V',@j))
		SET @j=@j+1;
		END
END

