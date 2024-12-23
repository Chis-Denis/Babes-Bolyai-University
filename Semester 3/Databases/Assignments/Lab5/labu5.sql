-- 1. Create Table AircraftModelNew (AircraftModel)
CREATE TABLE AircraftModelNew (
    ModelID INT PRIMARY KEY,       -- Primary key for AircraftModel
    ModelName NVARCHAR(100),       -- Aircraft Model Name
    ManufacturerID INT,            -- Foreign key for Manufacturer (you can add Manufacturer table)
    YearOfProduction INT          -- Year the model was produced
);

-- 2. Create Table EmployeeNew (Employee)
CREATE TABLE EmployeeNew (
    EmployeeID INT PRIMARY KEY,    -- Primary key for Employee
    FirstName NVARCHAR(50),        -- Employee First Name
    LastName NVARCHAR(50),         -- Employee Last Name
    Age INT,                       -- Employee Age
    HireDate DATE,                 -- Employee Hire Date
    Position NVARCHAR(50),         -- Employee Position
    DepartmentID INT               -- Foreign key for Department (you can add Department table)
);

-- 3. Create Table Aircraft_ServiceNew (Relationship between Aircraft and Employee)
CREATE TABLE Aircraft_ServiceNew (
    ServiceID INT PRIMARY KEY,     -- Service record ID
    AircraftID INT,                -- Aircraft ID
    EmployeeID INT,                -- Employee ID (providing service)
    ServiceDate DATE,              -- Date the service was performed
    ServiceDescription NVARCHAR(255), -- Description of the service
    FOREIGN KEY (AircraftID) REFERENCES Aircraft(AircraftID),  -- Aircraft foreign key
    FOREIGN KEY (EmployeeID) REFERENCES Employee(EmployeeID)   -- Employee foreign key
);

-- 4. Insert Data into AircraftModelNew (Aircraft Models)
INSERT INTO AircraftModelNew (ModelID, ModelName, ManufacturerID, YearOfProduction) VALUES
(1, 'Boeing 737', 1, 1998),
(2, 'Airbus A320', 2, 2000),
(3, 'Cessna Citation', 3, 2005),
(4, 'Bombardier CRJ200', 4, 2008),
(5, 'Embraer E175', 5, 2012);

-- 5. Create Non-Clustered Index on AircraftModelNew (for efficient search by ModelName)
CREATE NONCLUSTERED INDEX ix_aircraft_model_name_new ON AircraftModelNew(ModelName);

-- 10. Select All Data from aircraft_employee_details_new View
--clustered index scan
SELECT * FROM aircraft_employee_details_new;

-- 11. Select Specific Data from AircraftModelNew (Aircraft Models)
--clustered index seek
SELECT * FROM AircraftModelNew WHERE ModelID = 3;

-- 12. Select Aircraft Model Names from AircraftModelNew 
--nonclustered index scan
SELECT ModelName FROM AircraftModelNew;

-- 13. Select ModelID from AircraftModelNew where Aircraft Model is 'Boeing 737'
--nonclustered index seek
SELECT ModelID FROM AircraftModelNew WHERE ModelName = 'Boeing 737';

-- 14. Select ModelID, ModelName from AircraftModelNew where Aircraft Model is 'Airbus A320'
--keylookup
SELECT ModelID, ModelName FROM AircraftModelNew WHERE ModelName = 'Airbus A320';

-- 6. Insert Data into EmployeeNew (Employees)
INSERT INTO EmployeeNew (EmployeeID, FirstName, LastName, Age, HireDate, Position, DepartmentID) VALUES
(1, 'John', 'Doe', 28, '2015-06-01', 'Technician', 1),
(2, 'Jane', 'Smith', 35, '2010-04-15', 'Manager', 2),
(3, 'Alice', 'Johnson', 45, '2008-07-23', 'Technician', 1),
(4, 'Bob', 'Davis', 32, '2016-08-30', 'Technician', 1),
(5, 'Charlie', 'Wilson', 38, '2014-03-11', 'Manager', 2);

-- 7. Create Non-Clustered Index on EmployeeNew (for efficient search by Age)
CREATE NONCLUSTERED INDEX ix_employee_age_new ON EmployeeNew(Age);

-- 15. Select from EmployeeNew using the index created on Age (B)
--clustered index on Age 
SELECT * FROM EmployeeNew WHERE Age = 30;

-- 8. Insert Data into Aircraft_ServiceNew (Aircraft-Employee Relationship for Services)
INSERT INTO Aircraft_ServiceNew (ServiceID, AircraftID, EmployeeID, ServiceDate, ServiceDescription) VALUES
(1, 1, 1, '2023-05-15', 'Routine Maintenance'),
(2, 2, 2, '2023-06-20', 'Engine Overhaul'),
(3, 3, 3, '2023-07-10', 'Landing Gear Check');

-- 9. Create View for Aircraft and Employee Details (Join Aircraft Models, Employees, and Services)
CREATE VIEW aircraft_employee_details_new AS
SELECT
    am.ModelName AS AircraftModelName,
    e.Age AS EmployeeAge,
    aser.ServiceDescription AS ServiceDescription
FROM AircraftModelNew am
INNER JOIN Aircraft_ServiceNew aser ON am.ModelID = aser.AircraftID
INNER JOIN EmployeeNew e ON aser.EmployeeID = e.EmployeeID;

SELECT * FROM aircraft_employee_details_new;




-- Drop the index on AircraftModelNew
DROP INDEX ix_aircraft_model_name_new ON AircraftModelNew;

-- Drop the index on EmployeeNew
DROP INDEX ix_employee_age_new ON EmployeeNew;

-- Drop the view first if you created it
DROP VIEW IF EXISTS aircraft_employee_details_new;

-- Drop the dependent table (Aircraft_ServiceNew) first
DROP TABLE IF EXISTS Aircraft_ServiceNew;

-- Drop the tables (AircraftModelNew and EmployeeNew) after dropping the dependent table
DROP TABLE IF EXISTS AircraftModelNew;
DROP TABLE IF EXISTS EmployeeNew;