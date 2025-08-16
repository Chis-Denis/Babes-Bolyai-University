USE Spotify
GO

CREATE PROCEDURE insertIntoTestTables
AS
	BEGIN
		INSERT INTO TestTables(TestID,TableID,NoOfRows,Position)
		VALUES
		(1,1,10,3),
		(2,1,100,3),
		(3,1,1000,3),
		(4,2,10,1),
		(5,2,100,1),
		(6,2,1000,1),
		(7,3,10,2),
		(8,3,100,2),
		(9,3,1000,2);
END