USE Spotify
GO

CREATE PROCEDURE insertIntoTestViews
AS
	BEGIN
		INSERT INTO TestViews(TestID,ViewID)
		VALUES
		(1,1),
		(2,2),
		(3,3),
		(4,1),
		(5,2),
		(6,3),
		(7,1),
		(8,2),
		(9,3);
	END
