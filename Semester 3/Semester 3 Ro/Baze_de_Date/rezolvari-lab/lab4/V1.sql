USE Spotify
GO
--un view ce conţine o comandă SELECT pe o tabelă
CREATE View V1 AS
	SELECT DrepturiAutor
	FROM Contracte
	WHERE Onorariu>100
GO
