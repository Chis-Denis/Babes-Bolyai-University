-- Interleaved execution scenario for Parking table
-- Initial value: Name = 'Unirii' for ParkingID = 3

--
-- Scenario 1: Both T1 and T2 under READ COMMITTED
-- (Default isolation level)
-- In each window, you can run:
-- SET TRANSACTION ISOLATION LEVEL READ COMMITTED;

-- T1 (Window 1):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
SELECT Name FROM Parking WHERE ParkingID = 3;
-- (Do not commit or update yet)

-- T2 (Window 2):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
UPDATE Parking SET Name = 'Cipariu' WHERE ParkingID = 3;
COMMIT TRAN;

-- T1 (Window 1):
UPDATE Parking SET Name = 'Gara' WHERE ParkingID = 3;
COMMIT TRAN;

--
-- Scenario 2: T1 under SNAPSHOT, T2 under READ COMMITTED
-- (Assumes SNAPSHOT isolation is enabled for the database)
--
-- T1 (Window 1):
SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRAN;
SELECT Name FROM Parking WHERE ParkingID = 3;
-- (Do not commit or update yet)

-- T2 (Window 2):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
UPDATE Parking SET Name = 'Cipariu' WHERE ParkingID = 3;
COMMIT TRAN;

-- T1 (Window 1):
UPDATE Parking SET Name = 'Gara' WHERE ParkingID = 3;
COMMIT TRAN;

--
-- Scenario 3: T1 under REPEATABLE READ, T2 under READ COMMITTED
--
-- T1 (Window 1):
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
BEGIN TRAN;
SELECT Name FROM Parking WHERE ParkingID = 3;
-- (Do not commit or update yet)

-- T2 (Window 2):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
UPDATE Parking SET Name = 'Cipariu' WHERE ParkingID = 3;
-- This statement will be blocked until T1 commits or rolls back
COMMIT TRAN;

-- T1 (Window 1):
UPDATE Parking SET Name = 'Gara' WHERE ParkingID = 3;
COMMIT TRAN;

-- Schema definition for Parking table
CREATE TABLE Parking (
    ParkingID INT PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    City NVARCHAR(100) NOT NULL,
    Capacity INT NOT NULL
);
-- ParkingID is the search key of the clustered index on Parking (default for primary key in SQL Server)

-- Insert initial data for Parking
INSERT INTO Parking (ParkingID, Name, City, Capacity) VALUES
(1, 'Central', 'Bucharest', 100),
(2, 'Nord', 'Bucharest', 80),
(3, 'Unirii', 'Bucharest', 120), -- This is the row used in the concurrency scenarios
(4, 'Gara', 'Cluj', 60);
