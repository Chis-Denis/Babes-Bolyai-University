-- Table to track all database operations and their status
IF OBJECT_ID('Log', 'U') IS NULL
CREATE TABLE Log
(
	LogID INT PRIMARY KEY IDENTITY,
	Action NVARCHAR(100),
	Status NVARCHAR(100),
	Message NVARCHAR(100),
	CreatedAt DATETIME DEFAULT GETDATE()
);

-- Strict procedure that adds aircraft, model, and manufacturer in a single transaction (all or nothing)
CREATE OR ALTER PROCEDURE AddAircraftWithModel_Strict
	@TailNumber NVARCHAR(10),
	@ModelName NVARCHAR(100),
	@ManufacturerName NVARCHAR(100),
	@Country NVARCHAR(50),
	@YearOfProduction INT,
	@PurchaseDate DATE,
	@Price DECIMAL(18, 2)
AS
BEGIN 
	BEGIN TRY
		BEGIN TRANSACTION;

		-- Declare variables only once
		DECLARE @ManufacturerID INT;
		DECLARE @ModelID INT;

		-- Check if manufacturer exists, if not create it
		SELECT @ManufacturerID = ManufacturerID 
		FROM Manufacturer 
		WHERE Name = @ManufacturerName;

		IF @ManufacturerID IS NULL
		BEGIN
			INSERT INTO Manufacturer (Name, Country) 
			VALUES (@ManufacturerName, @Country);
			SET @ManufacturerID = SCOPE_IDENTITY();
		END

		-- Create new aircraft model
		INSERT INTO AircraftModel (ModelName, ManufacturerID, YearOfProduction)
		VALUES (@ModelName, @ManufacturerID, @YearOfProduction);
		SET @ModelID = SCOPE_IDENTITY();

		-- Add the actual aircraft
		INSERT INTO Aircraft (TailNumber, ModelID, PurchaseDate, Price)
		VALUES (@TailNumber, @ModelID, @PurchaseDate, @Price);

		-- Log successful operation
		INSERT INTO Log(Action, Status, Message) 
		VALUES ('AddAircraftWithModel_Strict', 'Success', 'Aircraft and related data added');

		COMMIT;
	END TRY
	BEGIN CATCH
		-- If anything fails, rollback and log the error
		ROLLBACK;
		INSERT INTO Log(Action, Status, Message) 
		VALUES ('AddAircraftWithModel_Strict', 'Failure', ERROR_MESSAGE());
	END CATCH
END;

-- Test the strict procedure with a Boeing 737
EXEC AddAircraftWithModel_Strict
	@TailNumber = 'N12345',
	@ModelName = 'Boeing 737',
	@ManufacturerName = 'Boeing',
	@Country = 'USA',
	@YearOfProduction = 2020,
	@PurchaseDate = '2020-01-01',
	@Price = 85000000.00;

-- Lenient procedure that handles each operation separately (can partially succeed)
CREATE OR ALTER PROCEDURE AddAircraftWithModel_Lenient
	@TailNumber NVARCHAR(10),
	@ModelName NVARCHAR(100),
	@ManufacturerName NVARCHAR(100),
	@Country NVARCHAR(50),
	@YearOfProduction INT,
	@PurchaseDate DATE,
	@Price DECIMAL(18, 2)
AS
BEGIN
	DECLARE @ManufacturerID INT = NULL;
	DECLARE @ModelID INT = NULL;

	-- First try to add/get manufacturer
	BEGIN TRY
		BEGIN TRANSACTION;
		SELECT @ManufacturerID = ManufacturerID 
		FROM Manufacturer 
		WHERE Name = @ManufacturerName;

		IF @ManufacturerID IS NULL
		BEGIN
			INSERT INTO Manufacturer (Name, Country) 
			VALUES (@ManufacturerName, @Country);
			SET @ManufacturerID = SCOPE_IDENTITY();
		END
		COMMIT;
		INSERT INTO Log(Action, Status, Message) 
		VALUES ('AddAircraftWithModel_Lenient', 'Success', 'Manufacturer added/retrieved');
	END TRY
	BEGIN CATCH
		ROLLBACK;
		INSERT INTO Log(Action, Status, Message) 
		VALUES ('AddAircraftWithModel_Lenient', 'Failure', 'Manufacturer operation failed: ' + ERROR_MESSAGE());
	END CATCH

	-- If manufacturer was added successfully, try to add model
	IF @ManufacturerID IS NOT NULL
	BEGIN
		BEGIN TRY
			BEGIN TRANSACTION;
			INSERT INTO AircraftModel (ModelName, ManufacturerID, YearOfProduction)
			VALUES (@ModelName, @ManufacturerID, @YearOfProduction);
			SET @ModelID = SCOPE_IDENTITY();
			COMMIT;
			INSERT INTO Log(Action, Status, Message) 
			VALUES ('AddAircraftWithModel_Lenient', 'Success', 'Model added');
		END TRY
		BEGIN CATCH
			ROLLBACK;
			INSERT INTO Log(Action, Status, Message) 
			VALUES ('AddAircraftWithModel_Lenient', 'Failure', 'Model operation failed: ' + ERROR_MESSAGE());
		END CATCH
	END

	-- If model was added successfully, try to add aircraft
	IF @ModelID IS NOT NULL
	BEGIN
		BEGIN TRY
			BEGIN TRANSACTION;
			INSERT INTO Aircraft (TailNumber, ModelID, PurchaseDate, Price)
			VALUES (@TailNumber, @ModelID, @PurchaseDate, @Price);
			COMMIT;
			INSERT INTO Log(Action, Status, Message) 
			VALUES ('AddAircraftWithModel_Lenient', 'Success', 'Aircraft added');
		END TRY
		BEGIN CATCH
			ROLLBACK;
			INSERT INTO Log(Action, Status, Message) 
			VALUES ('AddAircraftWithModel_Lenient', 'Failure', 'Aircraft operation failed: ' + ERROR_MESSAGE());
		END CATCH
	END
END;

-- Test the lenient procedure with an Airbus A320
EXEC AddAircraftWithModel_Lenient
	@TailNumber = 'N67890',
	@ModelName = 'Airbus A320',
	@ManufacturerName = 'Airbus',
	@Country = 'France',
	@YearOfProduction = 2021,
	@PurchaseDate = '2021-01-01',
	@Price = 95000000.00;

-- Create test data for transaction isolation level tests
INSERT INTO Aircraft (TailNumber, ModelID, PurchaseDate, Price)
VALUES ('TEST001', 1, '2023-01-01', 1000000.00);

-- Test for DIRTY READ (reading uncommitted data)
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
BEGIN TRAN;
UPDATE Aircraft SET Price = Price - 100000 WHERE TailNumber = 'TEST001';

-- Session 2: Demonstrates dirty read by seeing uncommitted changes
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED; --Fix: SET TRANSACTION ISOLATION LEVEL READ COMMITTED
SELECT * FROM Aircraft WHERE TailNumber = 'TEST001';

-- Test for NON-REPEATABLE READ (data changes between reads)
SET TRANSACTION ISOLATION LEVEL READ COMMITTED; -- Fix: Use repeatable read in session 1
BEGIN TRAN;
SELECT Price FROM Aircraft WHERE TailNumber = 'TEST001';
WAITFOR DELAY '00:00:10';
SELECT Price FROM Aircraft WHERE TailNumber = 'TEST001';
COMMIT;

-- Session 2: Changes data during the delay
UPDATE Aircraft SET Price = Price - 50000 WHERE TailNumber = 'TEST001';

-- Test for PHANTOM READ (new rows appear between reads)
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; -- Fix: SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
BEGIN TRAN;
SELECT TailNumber, Price FROM Aircraft WHERE Price > 500000;
WAITFOR DELAY '00:00:10';
SELECT TailNumber, Price FROM Aircraft WHERE Price > 500000;
COMMIT;

-- Session 2: Adds new row during the delay
INSERT INTO Aircraft (TailNumber, ModelID, PurchaseDate, Price)
VALUES ('TEST002', 1, '2023-01-01', 600000.00);

-- Test for DEADLOCK (two transactions waiting for each other)
-- Session 1: Updates first aircraft
BEGIN TRAN;
UPDATE Aircraft SET Price = Price + 100000 WHERE TailNumber = 'TEST001';
WAITFOR DELAY '00:00:10';

-- Session 2: Updates second aircraft
BEGIN TRAN;
UPDATE Aircraft SET Price = Price + 200000 WHERE TailNumber = 'TEST002';
WAITFOR DELAY '00:00:10';

-- Session 1: Tries to update second aircraft (causing deadlock)
UPDATE Aircraft SET Price = Price - 50000 WHERE TailNumber = 'TEST002';

-- Session 2: Tries to update first aircraft (causing deadlock)
UPDATE Aircraft SET Price = Price - 50000 WHERE TailNumber = 'TEST001';