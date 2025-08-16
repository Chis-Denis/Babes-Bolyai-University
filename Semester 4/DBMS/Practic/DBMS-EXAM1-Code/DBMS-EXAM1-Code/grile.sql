-- Schema definition for DentalCabinet table
CREATE TABLE DentalCabinet (
    DentalCabinetID INT PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    PhoneNo NVARCHAR(20) NOT NULL,
    Website NVARCHAR(200)
);
-- DentalCabinetID is the search key of the clustered index on DentalCabinet (default for primary key in SQL Server)

-- Initial value for concurrency scenario:
-- The value of Name for DentalCabinetID = 2 is 'Smile Dent' when T1 begins execution.

-- Interleaved execution scenario for DentalCabinetID = 2

-- T1 (Window 1):
BEGIN TRAN;
UPDATE DentalCabinet SET Name = 'Anna Dent' WHERE DentalCabinetID = 2;
-- (Do not commit yet)

-- T2 (Window 2):
BEGIN TRAN;
SELECT Name FROM DentalCabinet WHERE DentalCabinetID = 2;
-- (Do not commit yet)

-- T1 (Window 1):
COMMIT TRAN;

-- T2 (Window 2):
UPDATE DentalCabinet SET Name = 'Dental Spa' WHERE DentalCabinetID = 2;
COMMIT TRAN;

--
-- Q4: T1 under SERIALIZABLE, T2 under READ UNCOMMITTED
-- T1 (Window 1):
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
BEGIN TRAN;
UPDATE DentalCabinet SET Name = 'Anna Dent' WHERE DentalCabinetID = 2;
-- (Do not commit yet)

-- T2 (Window 2):
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
BEGIN TRAN;
SELECT Name FROM DentalCabinet WHERE DentalCabinetID = 2;
-- Expected result: Anna Dent (dirty read)
COMMIT TRAN;

--
-- Q5: T1 and T2 under READ COMMITTED
-- T1 (Window 1):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
UPDATE DentalCabinet SET Name = 'Anna Dent' WHERE DentalCabinetID = 2;
COMMIT TRAN;

-- T2 (Window 2):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
UPDATE DentalCabinet SET Name = 'Dental Spa' WHERE DentalCabinetID = 2;
COMMIT TRAN;
-- Final value: Dental Spa

--
-- Q6: T1 and T2 under SNAPSHOT
-- (Assumes SNAPSHOT isolation is enabled for the database)
-- T1 (Window 1):
SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRAN;
UPDATE DentalCabinet SET Name = 'Anna Dent' WHERE DentalCabinetID = 2;
-- (Do not commit yet)

-- T2 (Window 2):
SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRAN;
SELECT Name FROM DentalCabinet WHERE DentalCabinetID = 2;
UPDATE DentalCabinet SET Name = 'Dental Spa' WHERE DentalCabinetID = 2;
COMMIT TRAN;
-- Final value: Dental Spa (T1's commit will fail with a write conflict if attempted after T2)

-- Insert initial data for DentalCabinet
INSERT INTO DentalCabinet (DentalCabinetID, Name, PhoneNo, Website) VALUES
(1, 'City Dental', '021-111-1111', 'www.citydental.com'),
(2, 'Smile Dent', '021-222-2222', 'www.smiledent.com');
-- Row with DentalCabinetID = 2 is used in concurrency scenarios
