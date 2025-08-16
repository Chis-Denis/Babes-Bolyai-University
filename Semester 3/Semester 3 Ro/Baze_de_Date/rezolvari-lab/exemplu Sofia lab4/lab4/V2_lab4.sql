USE Spotify
GO

--un view ce conţine o comandă SELECT aplicată pe cel puţin două tabele,
CREATE VIEW V2 AS
	SELECT A.Nume,C.Onorariu
	FROM Artisti A
	INNER JOIN
	Contracte C
	ON A.AId=C.AId
	WHERE Onorariu>1000
	GO
