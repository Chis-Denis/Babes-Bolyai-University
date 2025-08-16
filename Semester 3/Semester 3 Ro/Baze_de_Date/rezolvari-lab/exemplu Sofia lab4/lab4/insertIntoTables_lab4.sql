USE Spotify
GO

--procedura stocata insereaza valori in tabelul Tables
--numele tabelului de inserat este dat ca si parametru
CREATE PROCEDURE insertIntoTables
(@tableName VARCHAR(40))
AS
BEGIN
	INSERT INTO Tables(Name)
	VALUES
	(@tableName);
END;	