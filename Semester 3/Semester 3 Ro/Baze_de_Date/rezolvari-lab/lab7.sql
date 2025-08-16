
CREATE FUNCTION validate_parameter(@param INT)
RETURNS INT
AS
BEGIN
	DECLARE @valid INT=1;
	IF (@param <= 0)
	BEGIN
		SET @valid = 0;
	END
	RETURN @valid;
END
GO

-- CREATE TABLE Profesori(
-- Pid INT PRIMARY KEY IDENTITY,
-- Nume VARCHAR(50),
-- Prenume VARCHAR(50),
-- Studii VARCHAR(50)
-- )

CREATE OR ALTER PROCEDURE CRUD_Profesori(@cantitate INT)
AS
BEGIN

	 IF (dbo.validate_parameter(@cantitate)=0)
	 BEGIN
		print 'invalid quantity given'
		RETURN;
	END
	ELSE
	BEGIN

		DECLARE @i INT;
		SET @i=1;

		DECLARE @Pid INT;
		SET @Pid=(SELECT MAX(Pid) FROM Profesori)
		SET @Pid=@Pid+1;
		IF @Pid is NULL SET @Pid=1;


		DECLARE @nume VARCHAR(30);
		DECLARE @prenume VARCHAR(30);
		DECLARE @studii VARCHAR(30);

		WHILE @i <= @cantitate 
		BEGIN

			SET @nume=CONVERT(VARCHAR(30),'nume'+ CONVERT(VARCHAR(4),@Pid))			
			SET @prenume=CONVERT(VARCHAR(30),'prenume'+ CONVERT(VARCHAR(4),@Pid))			
			SET @studii=CONVERT(VARCHAR(30),'studii'+ CONVERT(VARCHAR(4),@Pid))			
			INSERT INTO Profesori(Nume,Prenume,Studii) VALUES(@nume,@prenume,@studii)
			SET @Pid=@Pid+1
			SET @i=@i+1;
		END
	END

	SELECT * FROM Profesori

	UPDATE Profesori 
	SET Studii = 'UBB-FMI'
	WHERE Pid % 2 = 1
	SELECT * FROM Profesori

	DELETE FROM Profesori
	WHERE Pid % 2 =1
	SELECT * FROM Profesori

END



exec CRUD_Profesori 5;
DELETE FROM Profesori;


-- CREATE TABLE Orar(
-- Oid INT PRIMARY KEY IDENTITY,
-- Descrierre VARCHAR(50),
-- )


CREATE OR ALTER PROCEDURE CRUD_Orar(@cantitate INT)
AS
BEGIN

	 IF (dbo.validate_parameter(@cantitate)=0)
	 BEGIN
		print 'invalid quantity given'
		RETURN;
	END
	ELSE
	BEGIN

		DECLARE @i INT;
		SET @i=1;

		DECLARE @id INT;
		SET @id=(SELECT MAX(Oid) FROM Orar)
		SET @id=@id+1;
		IF @id is NULL SET @id=1;


		DECLARE @descriere VARCHAR(30);
		

		WHILE @i <= @cantitate 
		BEGIN

			SET @descriere=CONVERT(VARCHAR(30),'descriere'+ CONVERT(VARCHAR(4),@id))			
		
			INSERT INTO Orar(Descrierre) VALUES(@descriere)
			SET @id=@id+1
			SET @i=@i+1;
		END
	END


	SELECT * FROM Orar

	UPDATE Orar 
	SET Descrierre = 'Descrierre'
	WHERE Oid % 2 = 1
	SELECT * FROM Orar

	DELETE FROM Orar
	WHERE Oid % 2 =1
	SELECT * FROM Orar
END



IF EXISTS(SELECT NAME FROM sys.indexes WHERE name='S_idx_sgj_descriereorar')
DROP INDEX S_idx_sgj_descriereorar ON Orar
CREATE NONCLUSTERED INDEX S_idx_sgj_descriereorar ON Orar(Descrierre)


ALTER TABLE Orar
ADD CONSTRAINT chk_Orar CHECK (Descrierre > '')



exec CRUD_Orar 10;
SELECT * FROM Orar;

DELETE FROM Orar;

CREATE VIEW vw_Orar AS
SELECT * FROM Orar
WHERE Descrierre LIKE 'des%'


SELECT * FROM vw_Orar
ORDER BY Descrierre;


IF EXISTS(SELECT NAME FROM sys.indexes WHERE name='S_idx_sgj_numeprofesori')
DROP INDEX S_idx_sgj_numeprofesori ON Profesori
CREATE NONCLUSTERED INDEX S_idx_sgj_numeprofesori ON Profesori(Pid,Nume,Prenume,Studii)


exec CRUD_Profesori 10;
SELECT * FROM Profesori;
DELETE FROM Orar;

CREATE VIEW vw_Profesori AS
SELECT * FROM Profesori
WHERE Nume LIKE 'nume%'


SELECT * FROM vw_Profesori
ORDER BY Nume;

SELECT * FROM sys.indexes;


