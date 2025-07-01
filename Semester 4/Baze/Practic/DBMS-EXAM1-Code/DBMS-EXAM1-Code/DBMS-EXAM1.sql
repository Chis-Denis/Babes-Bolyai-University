-- Create Neighborhood table
CREATE TABLE Neighborhood (
    NeighborhoodID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    Location NVARCHAR(200) NOT NULL
);

-- Create MainResident table
CREATE TABLE MainResident (
    ResidentID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    PhoneNumber NVARCHAR(20) NOT NULL
);

-- Create SmartHome table
CREATE TABLE SmartHome (
    HomeID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    Address NVARCHAR(200) NOT NULL,
    NeighborhoodID INT NOT NULL,
    ResidentID INT NOT NULL,
    FOREIGN KEY (NeighborhoodID) REFERENCES Neighborhood(NeighborhoodID),
    FOREIGN KEY (ResidentID) REFERENCES MainResident(ResidentID)
);

-- Create SmartDevice table
CREATE TABLE SmartDevice (
    DeviceID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    Type NVARCHAR(50) NOT NULL
);

-- Create HomeDevice table (many-to-many relationship)
CREATE TABLE HomeDevice (
    HomeID INT NOT NULL,
    DeviceID INT NOT NULL,
    PRIMARY KEY (HomeID, DeviceID),
    FOREIGN KEY (HomeID) REFERENCES SmartHome(HomeID),
    FOREIGN KEY (DeviceID) REFERENCES SmartDevice(DeviceID)
);

-- Insert 10 Neighborhoods
INSERT INTO Neighborhood (Name, Location) VALUES
('Greenwood', 'Northside'),
('Sunnydale', 'Eastside'),
('Lakeshore', 'Westside'),
('Hilltop', 'Southside'),
('Maplewood', 'Central'),
('Brookfield', 'Northwest'),
('Riverside', 'Southeast'),
('Oakridge', 'Northeast'),
('Pinecrest', 'Southwest'),
('Cedar Grove', 'Downtown');

-- Insert 10 Main Residents
INSERT INTO MainResident (Name, PhoneNumber) VALUES
('Alice Smith', '555-1234'),
('Bob Johnson', '555-2345'),
('Carol Lee', '555-3456'),
('David Kim', '555-4567'),
('Eva Brown', '555-5678'),
('Frank White', '555-6789'),
('Grace Green', '555-7890'),
('Henry Black', '555-8901'),
('Ivy Young', '555-9012'),
('Jack King', '555-0123');

-- Insert 10 Smart Homes (NeighborhoodID and ResidentID from 1 to 10)
INSERT INTO SmartHome (Name, Address, NeighborhoodID, ResidentID) VALUES
('Home Alpha', '101 Main St', 1, 1),
('Home Beta', '102 Main St', 2, 2),
('Home Gamma', '103 Main St', 3, 3),
('Home Delta', '104 Main St', 4, 4),
('Home Epsilon', '105 Main St', 5, 5),
('Home Zeta', '106 Main St', 6, 6),
('Home Eta', '107 Main St', 7, 7),
('Home Theta', '108 Main St', 8, 8),
('Home Iota', '109 Main St', 9, 9),
('Home Kappa', '110 Main St', 10, 10);

-- Insert 10 Smart Devices
INSERT INTO SmartDevice (Name, Type) VALUES
('Thermostat', 'Climate'),
('Smart Lock', 'Security'),
('Light Bulb', 'Lighting'),
('Security Camera', 'Security'),
('Smart Speaker', 'Audio'),
('Smoke Detector', 'Safety'),
('Smart TV', 'Entertainment'),
('Water Sensor', 'Safety'),
('Garage Opener', 'Access'),
('Robot Vacuum', 'Cleaning');

-- Insert 10 HomeDevice relationships
INSERT INTO HomeDevice (HomeID, DeviceID) VALUES
(1, 1),
(2, 2),
(3, 3),
(4, 4),
(5, 5),
(6, 6),
(7, 7),
(8, 8),
(9, 9),
(10, 10);

-- Non-Repeatable Read Concurrency Issue Scenario

-- Transaction 1 (Window 1):
BEGIN TRANSACTION;
SELECT * FROM Neighborhood WHERE NeighborhoodID = 1;
-- (Do not commit or rollback yet)

-- Transaction 2 (Window 2):
UPDATE Neighborhood SET Name = 'ChangedName' WHERE NeighborhoodID = 1;
COMMIT;

-- Back to Transaction 1 (Window 1):
SELECT * FROM Neighborhood WHERE NeighborhoodID = 1;
-- Non-repeatable read: value has changed.

-- Solution: Use REPEATABLE READ isolation level
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
BEGIN TRANSACTION;
SELECT * FROM Neighborhood WHERE NeighborhoodID = 1;
-- (Now, Transaction 2's UPDATE will be blocked until Transaction 1 commits or rolls back)

--
-- Explanation:
-- Non-repeatable read occurs when a transaction reads the same row twice and gets different values because another transaction modified and committed changes to that row in between the two reads. This happens under the default READ COMMITTED isolation level, which does not prevent other transactions from modifying data between reads.
--
-- Solution:
-- Using the REPEATABLE READ isolation level prevents non-repeatable reads by placing shared locks on all rows read by the transaction. This ensures that no other transaction can modify or delete those rows until the first transaction completes (commits or rolls back), thus guaranteeing repeatable reads.

--
-- How to run the above scenario:
--
-- 1. Open two separate SSMS windows (Window 1 and Window 2).
--
-- 2. In Window 1, run the first block (BEGIN TRANSACTION; SELECT ...). Do NOT commit or rollback yet.
--
-- 3. In Window 2, run the UPDATE and COMMIT block. This will succeed and commit the change.
--
-- 4. Go back to Window 1 and run the SELECT again. You will see the changed value (non-repeatable read).
--
-- 5. To see the solution, in Window 1, first run the SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; line, then BEGIN TRANSACTION; and SELECT ...
--
-- 6. Now, in Window 2, try to run the UPDATE. It will be blocked until you commit or rollback in Window 1. This prevents the non-repeatable read.
