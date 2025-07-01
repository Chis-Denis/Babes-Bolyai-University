-- Create DentalCabinet table
CREATE TABLE DentalCabinet (
    DentalCabinetID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    PhoneNumber NVARCHAR(20) NOT NULL,
    Website NVARCHAR(200)
);

-- Create Client table
CREATE TABLE Client (
    ClientID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    Age INT NOT NULL
);

-- Create Appointment table (many-to-many between Client and DentalCabinet)
CREATE TABLE Appointment (
    AppointmentID INT IDENTITY(1,1) PRIMARY KEY,
    ClientID INT NOT NULL,
    DentalCabinetID INT NOT NULL,
    Date DATETIME NOT NULL,
    Price DECIMAL(10,2) NOT NULL,
    FOREIGN KEY (ClientID) REFERENCES Client(ClientID),
    FOREIGN KEY (DentalCabinetID) REFERENCES DentalCabinet(DentalCabinetID)
);

-- Create MedicalImage table
CREATE TABLE MedicalImage (
    MedicalImageID INT IDENTITY(1,1) PRIMARY KEY,
    ClientID INT NOT NULL,
    DateTaken DATETIME NOT NULL,
    FOREIGN KEY (ClientID) REFERENCES Client(ClientID)
);

-- Create ImageStore table
CREATE TABLE ImageStore (
    ImageStoreID INT IDENTITY(1,1) PRIMARY KEY,
    MedicalImageID INT NOT NULL,
    Description NVARCHAR(500),
    FOREIGN KEY (MedicalImageID) REFERENCES MedicalImage(MedicalImageID)
);

-- Create Dentist table
CREATE TABLE Dentist (
    DentistID INT IDENTITY(1,1) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    Specialty NVARCHAR(100) NOT NULL,
    DentalCabinetID INT NOT NULL,
    FOREIGN KEY (DentalCabinetID) REFERENCES DentalCabinet(DentalCabinetID)
);

-- Insert DentalCabinets
INSERT INTO DentalCabinet (Name, PhoneNumber, Website) VALUES
('SmileCare', '021-111-1111', 'www.smilecare.com'),
('BrightTeeth', '021-222-2222', 'www.brightteeth.com'),
('FamilyDental', '021-333-3333', 'www.familydental.com'),
('CityDent', '021-444-4444', 'www.citydent.com'),
('HappyMouth', '021-555-5555', 'www.happymouth.com');

-- Insert Clients
INSERT INTO Client (Name, Age) VALUES
('Alice Pop', 28),
('Bob Ionescu', 35),
('Carmen Vasilescu', 42),
('Dan Georgescu', 30),
('Elena Stan', 25);

-- Insert Dentists (each belongs to a DentalCabinetID 1-5)
INSERT INTO Dentist (Name, Specialty, DentalCabinetID) VALUES
('Dr. Mihai', 'Orthodontics', 1),
('Dr. Ana', 'Pediatric Dentistry', 2),
('Dr. Vlad', 'Endodontics', 3),
('Dr. Ioana', 'Prosthodontics', 4),
('Dr. Radu', 'Periodontics', 5);

-- Insert Appointments (linking ClientID 1-5 and DentalCabinetID 1-5)
INSERT INTO Appointment (ClientID, DentalCabinetID, Date, Price) VALUES
(1, 1, '2024-06-01 09:00', 200.00),
(2, 2, '2024-06-02 10:30', 150.00),
(3, 3, '2024-06-03 11:00', 300.00),
(4, 4, '2024-06-04 12:00', 250.00),
(5, 5, '2024-06-05 13:30', 180.00);

-- Insert MedicalImages (each for a ClientID 1-5)
INSERT INTO MedicalImage (ClientID, DateTaken) VALUES
(1, '2024-05-01'),
(2, '2024-05-02'),
(3, '2024-05-03'),
(4, '2024-05-04'),
(5, '2024-05-05');

-- Insert ImageStore (each for a MedicalImageID 1-5)
INSERT INTO ImageStore (MedicalImageID, Description) VALUES
(1, 'Panoramic X-ray'),
(2, 'Bitewing X-ray'),
(3, 'Periapical X-ray'),
(4, '3D Scan'),
(5, 'Orthopantomogram');

--
-- Dirty Read Concurrency Issue Scenario
--
-- Transaction 1 (Window 1):
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
BEGIN TRAN;
SELECT Name FROM Client WHERE ClientID = 1;
-- (Do not commit or rollback yet)

-- Transaction 2 (Window 2):
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
BEGIN TRAN;
UPDATE Client SET Name = 'DirtyName' WHERE ClientID = 1;
-- (Do not commit or rollback yet)

-- Back to Transaction 1 (Window 1):
SELECT Name FROM Client WHERE ClientID = 1;
-- Transaction 1 can see the uncommitted value 'DirtyName' (dirty read)

-- Solution: Use READ COMMITTED or higher isolation level
--
-- Transaction 1 (Window 1):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
SELECT Name FROM Client WHERE ClientID = 1;
-- (Do not commit or rollback yet)

-- Transaction 2 (Window 2):
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
BEGIN TRAN;
UPDATE Client SET Name = 'DirtyName' WHERE ClientID = 1;
-- (Do not commit or rollback yet)

-- Back to Transaction 1 (Window 1):
SELECT Name FROM Client WHERE ClientID = 1;
-- Transaction 1 will NOT see the uncommitted value (no dirty read)

-- Explanation:
-- Dirty reads occur when a transaction reads data that has been modified by another transaction but not yet committed. This can lead to inconsistent or incorrect results if the second transaction is rolled back. Using READ COMMITTED or a higher isolation level prevents dirty reads by ensuring that only committed data is visible to other transactions.
