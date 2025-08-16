USE Spotify
GO
--un view ce conţine o comandă SELECT aplicată pe cel puţin două tabele şi având o clauză GROUP BY
CREATE VIEW V3 AS
	SELECT A.Nume,
	COUNT(A.Nume) AS NrContracte
	FROM Artisti A
	INNER JOIN
	Contracte C
	ON A.AId=C.AId
	GROUP BY A.Nume
	
GO
