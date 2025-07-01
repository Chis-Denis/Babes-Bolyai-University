CREATE TABLE Log
(
	LogID INT PRIMARY KEY IDENTITY,
	Action NVARCHAR(100),
	Status NVARCHAR(100),
	Message NVARCHAR(4000),
	CreatedAt DATETIME DEFAULT GETDATE()
);

CREATE TABLE Airplane
(
    AirplaneID INT PRIMARY KEY IDENTITY,
	Model NVARCHAR(50) NOT NULL
);

CREATE TABLE Pilot
(
	PilotID INT PRIMARY KEY IDENTITY,
	Name NVARCHAR(50) NOT NULL
);

CREATE TABLE AirplanePilot (
    AirplaneID INT,
    PilotID INT,
    PRIMARY KEY (AirplaneID, PilotID),
    FOREIGN KEY (AirplaneID) REFERENCES Airplane(AirplaneID),
    FOREIGN KEY (PilotID) REFERENCES Pilot(PilotID)
);

CREATE PROCEDURE AddAirplanesAndPilots_Strict
	@Model NVARCHAR(200),
	@PilotNames NVARCHAR(MAX)
	AS
	BEGIN 
		BEGIN TRY
			BEGIN TRANSACTION;

			INSERT INTO Airplane (Model) VALUES (@Model);
			DECLARE @AirplaneID INT = SCOPE_IDENTITY();

			DECLARE @Name NVARCHAR(100);
			DECLARE @Pos INT = 0;
			DECLARE @NextPos INT;
			WHILE LEN(@PilotNames) > 0
			BEGIN
				SET @NextPos = CHARINDEX(',', @PilotNames);
				IF @NextPos = 0
					SET @NextPos = LEN(@PilotNames) + 1;

				SET @Name = LTRIM(RTRIM(SUBSTRING(@PilotNames, 1, @NextPos - 1)));
				SET @PilotNames = SUBSTRING(@PilotNames, @NextPos + 1, LEN(@PilotNames));

				INSERT INTO Pilot (Name) VALUES (@Name);
				DECLARE @PilotID INT = SCOPE_IDENTITY();

				INSERT INTO AirplanePilot (AirplaneID, PilotID) VALUES (@AirplaneID, @PilotID);
			END;

			INSERT INTO Log(Action, Status, Message) VALUES ('AddAirplanesAndPilots_Strict', 'Success', 'Airplane and pilots added');

			COMMIT;
		END TRY
		BEGIN CATCH
			ROLLBACK;
			INSERT INTO Log(Action, Status, Message) VALUES ('AddAirplanesAndPilots_Strict', 'Failure', ERROR_MESSAGE());
		END CATCH
END;

EXEC AddAirplanesAndPilots_Strict
	@Model = 'Boeing 737',
	@PilotNames= 'Alice Smith, Bob Johnson, Carol White';

SELECT * FROM Airplane;
SELECT * FROM Pilot;
SELECT * FROM AirplanePilot;
SELECT * FROM Log;

DECLARE @TooLongModel NVARCHAR(200) = REPLICATE('X', 60);  -- 60 characters, exceeds the Airplane.Model limit of 50
EXEC AddAirplanesAndPilots_Strict 
    @Model = @TooLongModel, 
    @PilotNames = 'A, B, C';

-- LENIENT PROCEDURE
CREATE PROCEDURE AddAirplanesAndPilots_Lenient
	@Model NVARCHAR(200),
	@PilotNames NVARCHAR (MAX)
AS
BEGIN
	DECLARE @AirplaneID INT = NULL;
	DECLARE @Name NVARCHAR(100);
	DECLARE @NextPos INT;
	DECLARE @PilotID INT;

	BEGIN TRY
		BEGIN TRANSACTION;

		INSERT INTO Airplane (Model) VALUES (@Model);
        SET @AirplaneID = SCOPE_IDENTITY();

        COMMIT;
        INSERT INTO Log(Action, Status, Message) 
        VALUES ('AddAirplanesAndPilots_Lenient_Transactional', 'Success', 'Airplane inserted');
    END TRY
	BEGIN CATCH
        ROLLBACK;
        INSERT INTO Log(Action, Status, Message) 
        VALUES ('AddAirplanesAndPilots_Lenient_Transactional', 'Failure', 'Airplane insert failed: ' + ERROR_MESSAGE());
    END CATCH
	
	WHILE LEN(@PilotNames) > 0
    BEGIN
        SET @NextPos = CHARINDEX(',', @PilotNames);
        IF @NextPos = 0 SET @NextPos = LEN(@PilotNames) + 1;

        SET @Name = LTRIM(RTRIM(SUBSTRING(@PilotNames, 1, @NextPos - 1)));
        SET @PilotNames = SUBSTRING(@PilotNames, @NextPos + 1, LEN(@PilotNames));
	
	BEGIN TRY
		BEGIN TRANSACTION;

		INSERT INTO Pilot (Name) VALUES (@Name);
		SET @PilotID = SCOPE_IDENTITY();

		INSERT INTO Log(Action, Status, Message) 
            VALUES ('AddAirplanesAndPilots_Lenient_Transactional', 'Success', 'Pilot "' + @Name + '" inserted');

		IF @AirplaneID IS NOT NULL
            BEGIN
                INSERT INTO AirplanePilot (AirplaneID, PilotID) VALUES (@AirplaneID, @PilotID);
                INSERT INTO Log(Action, Status, Message) 
                VALUES ('AddAirplanesAndPilots_Lenient_Transactional', 'Success', 'Linked pilot "' + @Name + '" to airplane');
            END

			COMMIT;
	END TRY

	BEGIN CATCH
		ROLLBACK;
		INSERT INTO Log(Action, Status, Message)
		VALUES ('AddAirplanesAndPilots_Lenient_Transactional', 'Failure', 'Pilot "' + @Name + '" failed: ' + ERROR_MESSAGE());
	END CATCH

	END
END;

EXEC AddAirplanesAndPilots_Lenient 
    @Model = 'Airbus A320', 
    @PilotNames = 'Robert C. Martin,John Doe';

DECLARE @TooLongModel2 NVARCHAR(200) = REPLICATE('X', 100);
DECLARE @FullPilotNames NVARCHAR(MAX) = 'Andy Hunt,' + @TooLongModel2;

EXEC AddAirplanesAndPilots_Lenient
@Model = 'Cessna 172', 
@PilotNames = @FullPilotNames;

SELECT * FROM Airplane;
SELECT * FROM Pilot;
SELECT * FROM AirplanePilot;
SELECT * FROM Log ORDER BY CreatedAt DESC;

--Now we create a testTable for the grade 9 part

-- Safe test table for flight hours
CREATE TABLE FlightAccounts (
    AccountID INT PRIMARY KEY IDENTITY,
    PilotName NVARCHAR(100),
    FlightHours INT
);

-- Add test data
INSERT INTO FlightAccounts (PilotName, FlightHours)
VALUES ('Alice', 1200), ('Bob', 1500);

  SELECT * FROM FlightAccounts;

--This is to test the DIRTY READ (e.g., uncommitted flight hour update)
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
BEGIN TRAN;
UPDATE FlightAccounts SET FlightHours = FlightHours - 10 WHERE PilotName = 'Alice';

-- Session 2: read uncommitted change (dirty read)
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED; --Fix : SET TRANSACTION ISOLATION LEVEL READ COMMITTED
SELECT * FROM FlightAccounts WHERE PilotName = 'Alice';

--To test NON-REPEATABLE READ
SET TRANSACTION ISOLATION LEVEL READ COMMITTED; -- Fix : Use repeatable read in sess 1
BEGIN TRAN;
SELECT FlightHours FROM FlightAccounts WHERE PilotName = 'Bob';
WAITFOR DELAY '00:00:10';
SELECT FlightHours FROM FlightAccounts WHERE PilotName = 'Bob';
COMMIT;

--do this in session 2 DURING the delay
UPDATE FlightAccounts SET FlightHours = 1234 WHERE PilotName = 'Bob';

SELECT * FROM FlightAccounts WHERE PilotName = 'Bob';

--PHANTOM READ - fix: SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
BEGIN TRAN;

-- Initially select rows for 'Bob' and 'Alice'
SELECT PilotName, FlightHours 
FROM FlightAccounts
WHERE FlightHours > 500;

WAITFOR DELAY '00:00:10';  -- Wait for session 2 to modify data

-- Select again (phantom read will be seen if rows have changed)
SELECT PilotName, FlightHours 
FROM FlightAccounts
WHERE FlightHours > 500;

COMMIT;

-- SESSION 2: Modify data while the delay is happening
	INSERT INTO FlightAccounts (PilotName, FlightHours) 
	VALUES ('Charlie', 600);

--DEADLOCK - fix: SET DEADLOCK_PRIORITY HIGH; for session 1 and SET DEADLOCK_PRIORITY Low; for session 2 OR access objects in same order

-- Run simultaneously in Window 1:
BEGIN TRANSACTION
    UPDATE Airplane SET Model = Model WHERE AirplaneID = 1
    WAITFOR DELAY '00:00:05'
    UPDATE AirplanePilot SET PilotID = PilotID WHERE AirplaneID = 1
COMMIT TRANSACTION
 
-- Run simultaneously in Window 2:
BEGIN TRANSACTION
    UPDATE AirplanePilot SET AirplaneID = AirplaneID WHERE AirplaneID = 1
    WAITFOR DELAY '00:00:05'
    UPDATE Airplane SET Model = Model WHERE AirplaneID = 1
COMMIT TRANSACTION
 
-- Solution (run in both windows after test):
BEGIN TRANSACTION
    UPDATE Airplane SET Model = Model WHERE AirplaneID = 1
    UPDATE AirplanePilot SET PilotID = PilotID WHERE AirplaneID = 1
COMMIT TRANSACTION

--for grade 10! - Fix : use versioning ADD Version INT DEFAULT 0; 

-- SESSION 1: Start Transaction and Read Data
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
SELECT FlightHours FROM FlightAccounts WHERE PilotName = 'Bob';
-- Assume the flight hours is 1500.

-- Modify the flight hours
UPDATE FlightAccounts
SET FlightHours = FlightHours - 10
WHERE PilotName = 'Bob';

-- Wait for session 2 to start
WAITFOR DELAY '00:00:10';

COMMIT;  -- Commit the update.

--SESSION 2
-- SESSION 2: Start Transaction and Read Data
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
SELECT FlightHours FROM FlightAccounts WHERE PilotName = 'Bob';
-- Assume the flight hours is still 1500 (no lock, so it doesn't see Session 1's changes).

-- Modify the flight hours
UPDATE FlightAccounts
SET FlightHours = FlightHours + 20
WHERE PilotName = 'Bob';

-- Wait for session 1 to commit, then attempt to commit this update.
WAITFOR DELAY '00:00:10';

COMMIT;  -- This will fail if Session 1 has already committed.