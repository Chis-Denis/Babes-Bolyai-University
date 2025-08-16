USE Spotify
GO

CREATE PROCEDURE Cantece_V2_100
(@TestId INT)
AS
	BEGIN
		DECLARE @t1 DATETIME,@t2 DATETIME,@t3 DATETIME
		DECLARE @nrInreg INT
		SELECT @nrInreg= NoOfRows FROM TestTables WHERE @TestId=TestID
		SET @t1=GETDATE();
		DELETE FROM Cantece;
		DELETE FROM Contracte;
		DELETE FROM Artisti;
		
		DECLARE @AId INT;
				SELECT @AId=MAX(AId) FROM Artisti;
				DECLARE @Albd INT;
				SELECT @Albd=MAX(AlbId) FROM Albume;
				DECLARE @PId INT;
				SELECT @PId=MAX(PId) FROM Playlisturi;
				INSERT INTO Cantece(CtcId,Denumire,Durata,AlbId,Aid,PId)
				VALUES
				(1,CONCAT('Denumire',1),2,@Albd,@AId,@PId)

		EXEC insertIntoArtisti @TestId
		EXEC insertIntoContracte
		EXEC insertIntoCantece @TestId
		SET @t2=GETDATE();
		SELECT * FROM V2;
		SET @t3=GETDATE();

		INSERT INTO TestRuns(Description,StartAt,EndAt)
		VALUES
		('DESCR Cantece_V2_100',@t1,@t3)

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

