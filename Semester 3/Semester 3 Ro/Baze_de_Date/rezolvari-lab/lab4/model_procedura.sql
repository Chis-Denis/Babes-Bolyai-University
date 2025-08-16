USE Spotify
GO

CREATE PROCEDURE Artisti_V1_10
(@TestId INT)
AS
	BEGIN
		
		DECLARE @t1 DATETIME,@t2 DATETIME,@t3 DATETIME
		DECLARE @nrInreg INT
		--setarea numarului de randuri pentru care se face testul
		SELECT @nrInreg= NoOfRows FROM TestTables WHERE @TestId=TestID
		--setarea t1-timpul de inceput a testului
		SET @t1=GETDATE();
		
		--stergerea din tabele
		DELETE FROM Cursuri;
		DELETE FROM Orar;
		
		--inserarea in tabele
		EXEC insertIntoOrar @TestId
		EXEC insertIntoCursuri @TestId

		--setarea t2 -timpul de dupa ce s-a terminat de sters si inserat in tabele
		SET @t2=GETDATE();

		SELECT * FROM V1;

		--setarea t3-timpul dupa ce s-au afisat view urile
		SET @t3=GETDATE();


		--inserarea in tabelele de test valorile necesare
		INSERT INTO TestRuns(Description,StartAt,EndAt)
		VALUES
		('DESCR ORAR_V1_10',@t1,@t3)

		DECLARE @TestRunID INT
		SELECT @TestRunID= MAX(TestRunID) FROM TestRuns
		DECLARE @ViewID INT
		SELECT @ViewID = ViewID FROM TestViews WHERE TestID=@TestId
		DECLARE @TableID INT
		SELECT @TableID = TableID FROM TestTables WHERE TestID=@TestId
		INSERT INTO TestRunViews(TestRunID,ViewID,StartAt,EndAt)
		VALUES
		(@TestRunID,@ViewID,@t2,@t3)

		INSERT INTO TestRunTables(TestRunID,TableID,StartAt,EndAt)
		VALUES
		(@TestRunID,@TableID,@t1,@t2)
		END

