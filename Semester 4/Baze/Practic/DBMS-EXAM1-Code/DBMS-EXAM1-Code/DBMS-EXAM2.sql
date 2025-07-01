-- Create Zone table
CREATE TABLE Zone (
    ZoneID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL
);

-- Create Category table
CREATE TABLE Category (
    CategoryID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL
);

-- Create ParkingLot table
CREATE TABLE ParkingLot (
    ParkingLotID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    MaxCapacity INT NOT NULL,
    ZoneID INT NOT NULL,
    CategoryID INT NOT NULL,
    FOREIGN KEY (ZoneID) REFERENCES Zone(ZoneID),
    FOREIGN KEY (CategoryID) REFERENCES Category(CategoryID)
);

-- Create Car table
CREATE TABLE Car (
    CarID INT IDENTITY(1,1) PRIMARY KEY,
    RegistrationNumber NVARCHAR(20) NOT NULL UNIQUE
);

-- Create Ticket table
CREATE TABLE Ticket (
    TicketID INT IDENTITY(1,1) PRIMARY KEY,
    ParkingLotID INT NOT NULL,
    CarID INT NOT NULL,
    ArrivalTime DATETIME NOT NULL,
    DepartureTime DATETIME NULL,
    FOREIGN KEY (ParkingLotID) REFERENCES ParkingLot(ParkingLotID),
    FOREIGN KEY (CarID) REFERENCES Car(CarID)
);

-- Insert sample Zones
INSERT INTO Zone (Name) VALUES
('City Center'),
('North District'),
('South District'),
('East District'),
('West District');

-- Insert sample Categories
INSERT INTO Category (Name) VALUES
('Open Air'),
('Underground'),
('Mall'),
('Garage'),
('Private');

-- Insert sample ParkingLots
INSERT INTO ParkingLot (Name, MaxCapacity, ZoneID, CategoryID) VALUES
('Central Park', 100, 1, 1),
('North Plaza', 50, 2, 2),
('South Mall', 80, 3, 3),
('East Garage', 60, 4, 4),
('West Private', 30, 5, 5);

-- Insert sample Cars
INSERT INTO Car (RegistrationNumber) VALUES
('B123XYZ'),
('C456ABC'),
('D789DEF'),
('E101GHI'),
('F202JKL');

-- Insert sample Tickets
INSERT INTO Ticket (ParkingLotID, CarID, ArrivalTime, DepartureTime) VALUES
(1, 1, '2024-06-01 08:00', '2024-06-01 10:00'),
(2, 2, '2024-06-01 09:00', '2024-06-01 11:00'),
(3, 3, '2024-06-01 07:30', '2024-06-01 08:30'),
(4, 4, '2024-06-01 10:15', NULL),
(5, 5, '2024-06-01 11:00', NULL);

-- Phantom Read Concurrency Issue Scenario

-- Transaction 1 (Window 1):
BEGIN TRANSACTION;
SELECT * FROM ParkingLot WHERE ZoneID = 1;
-- (Do not commit or rollback yet)

-- Transaction 2 (Window 2):
INSERT INTO ParkingLot (Name, MaxCapacity, ZoneID, CategoryID) VALUES ('New Lot', 40, 1, 1);
COMMIT;

-- Back to Transaction 1 (Window 1):
SELECT * FROM ParkingLot WHERE ZoneID = 1;
-- Phantom read: a new row appears that was not in the first read.

-- Solution: Use SERIALIZABLE isolation level
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
BEGIN TRANSACTION;
SELECT * FROM ParkingLot WHERE ZoneID = 1;
-- (Now, Transaction 2's INSERT will be blocked until Transaction 1 commits or rolls back)

-- How to run the above scenario:
-- 1. Open two separate SSMS windows (Window 1 and Window 2).
-- 2. In Window 1, run the first block (BEGIN TRANSACTION; SELECT ...). Do NOT commit or rollback yet.
-- 3. In Window 2, run the INSERT and COMMIT block. This will succeed and commit the new row.
-- 4. Go back to Window 1 and run the SELECT again. You will see the new row (phantom read).
-- 5. To see the solution, in Window 1, first run the SET TRANSACTION ISOLATION LEVEL SERIALIZABLE; line, then BEGIN TRANSACTION; and SELECT ...
-- 6. Now, in Window 2, try to run the INSERT. It will be blocked until you commit or rollback in Window 1. This prevents the phantom read.

-- Explanation:
-- Phantom reads occur when a transaction reads a set of rows that satisfy a condition, and another transaction inserts (or deletes) rows that satisfy the same condition before the first transaction completes. The first transaction then sees a different set of rows if it repeats the query. Using the SERIALIZABLE isolation level prevents this by locking the range of rows, so no new rows can be inserted or deleted until the transaction completes.
