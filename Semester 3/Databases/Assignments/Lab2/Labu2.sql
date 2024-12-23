INSERT INTO Customer (FirstName, LastName, PhoneNumber, Email)
VALUES ('John', 'Doe', '123-456-7890', 'john.doe@example.com');

INSERT INTO Manufacturer (Name, Country)
VALUES ('Boeing', 'USA');

INSERT INTO AircraftModel (ModelName, ManufacturerID, YearOfProduction)
VALUES ('737 Max', 1, 2018);

INSERT INTO AircraftModel (ModelName, ManufacturerID, YearOfProduction)
VALUES ('A320', 999, 2020);


UPDATE Aircraft
SET Price = Price * 1.1
WHERE ModelID = 1 AND Price < 1000000;

UPDATE Employee
SET Position = 'Senior Engineer'
WHERE HireDate BETWEEN '2010-01-01' AND '2015-12-31' AND Position LIKE '%Engineer%';


DELETE FROM Sale
WHERE SaleDate < '2020-01-01' AND EmployeeID IN (SELECT EmployeeID FROM Employee WHERE Position = 'Intern');

DELETE FROM ServiceHistory
WHERE Notes IS NULL;


--a)
SELECT FirstName, LastName FROM Customer
UNION
SELECT FirstName, LastName FROM Employee;

SELECT ModelName FROM AircraftModel
UNION ALL
SELECT Name FROM Manufacturer;

--b)
SELECT FirstName FROM Customer
INTERSECT
SELECT FirstName FROM Employee;

SELECT FirstName FROM Customer
WHERE FirstName IN (SELECT FirstName FROM Employee);

--c)
SELECT CustomerID FROM Customer
EXCEPT
SELECT CustomerID FROM Sale;

SELECT AircraftID FROM Aircraft
WHERE AircraftID NOT IN (SELECT AircraftID FROM ServiceHistory);

--d)
-- INNER JOIN between Sale, Customer, and Aircraft
SELECT C.FirstName, C.LastName, A.TailNumber, S.SaleDate
FROM Sale S
INNER JOIN Customer C ON S.CustomerID = C.CustomerID
INNER JOIN Aircraft A ON S.AircraftID = A.AircraftID;

-- LEFT JOIN between Employee and Department
SELECT E.FirstName, E.LastName, D.DepartmentName
FROM Employee E
LEFT JOIN Department D ON E.DepartmentID = D.DepartmentID;

-- RIGHT JOIN between Aircraft and AircraftModel
SELECT A.TailNumber, M.ModelName
FROM Aircraft A
RIGHT JOIN AircraftModel M ON A.ModelID = M.ModelID;

-- FULL JOIN between Manufacturer and AircraftModel
SELECT M.Name, AM.ModelName
FROM Manufacturer M
FULL JOIN AircraftModel AM ON M.ManufacturerID = AM.ManufacturerID;

--e)
SELECT * FROM Aircraft
WHERE ModelID IN (SELECT ModelID FROM AircraftModel WHERE YearOfProduction > 2015);

SELECT * FROM Sale
WHERE CustomerID IN (SELECT CustomerID FROM Customer WHERE Email LIKE '%yahoo');

--f)
--at least one sale
SELECT * FROM Customer C
WHERE EXISTS (SELECT 1 FROM Sale S WHERE S.CustomerID = C.CustomerID);

--at least one time serviced
SELECT * FROM Aircraft
WHERE EXISTS (SELECT 1 FROM ServiceHistory SH WHERE SH.AircraftID = Aircraft.AircraftID);

--g)
--average price of all aircraft
SELECT AVG(A.Price) AS AvgPrice
FROM (SELECT * FROM Aircraft WHERE PurchaseDate > '2018-01-01') A;

--number of sales for each e., join with first name
SELECT FirstName, TotalSales
FROM (SELECT EmployeeID, COUNT(*) AS TotalSales FROM Sale GROUP BY EmployeeID) T
JOIN Employee E ON T.EmployeeID = E.EmployeeID;

--h)
--more than 5 sales
SELECT CustomerID, COUNT(*) AS TotalSales
FROM Sale
GROUP BY CustomerID
HAVING COUNT(*) > 5;

--employee with more than 3 services
SELECT EmployeeID, COUNT(*) AS ServicesCount
FROM ServiceHistory
GROUP BY EmployeeID
HAVING COUNT(*) > 3;

--i)
--price aircraft higher than any service
SELECT * FROM Aircraft
WHERE Price > ANY (SELECT Cost FROM Service);

--all sales with date older than all services
SELECT * FROM Sale
WHERE SaleDate < ALL (SELECT ServiceDate FROM ServiceHistory);

--j)
-- increase of price of 10%
SELECT TailNumber, Price * 1.1 AS NewPrice FROM Aircraft;

-- unique employee positions
SELECT DISTINCT Position FROM Employee ORDER BY Position;

-- top 5
SELECT TOP 5 * FROM Customer ORDER BY CustomerID DESC;










INSERT INTO Customer (FirstName, LastName, PhoneNumber, Email)
VALUES 
    ('Alice', 'Johnson', '555-1234', 'alice.j@example.com'),
    ('Michael', 'Smith', '555-5678', 'michael.s@example.com'),
    ('Jennifer', 'Brown', '555-8765', 'jennifer.b@example.com'),
    ('David', 'Wilson', '555-4321', 'david.w@example.com'),
    ('Susan', 'Taylor', '555-9876', 'susan.t@example.com');

INSERT INTO Manufacturer (Name, Country)
VALUES 
    ('Boeing', 'USA'),
    ('Airbus', 'France'),
    ('Bombardier', 'Canada'),
    ('Embraer', 'Brazil'),
    ('Cessna', 'USA');

INSERT INTO AircraftModel (ModelName, ManufacturerID, YearOfProduction)
VALUES 
    ('737 MAX', 1, 2019),
    ('A320neo', 2, 2020),
    ('Global 7500', 3, 2018),
    ('E195-E2', 4, 2017),
    ('Citation Longitude', 5, 2018);

INSERT INTO Aircraft (TailNumber, ModelID, PurchaseDate, Price)
VALUES 
    ('N123AB', 1, '2021-01-15', 120000000.00),
    ('N456CD', 2, '2020-05-20', 110000000.00),
    ('N789EF', 3, '2019-08-30', 75000000.00),
    ('N321GH', 4, '2018-11-10', 60000000.00),
    ('N654IJ', 5, '2022-03-25', 28000000.00);

INSERT INTO Employee (FirstName, LastName, HireDate, Position, DepartmentID)
VALUES 
    ('Robert', 'King', '2015-06-12', 'Pilot', 1),
    ('Maria', 'Garcia', '2018-09-23', 'Engineer', 2),
    ('James', 'Martinez', '2017-12-01', 'Sales Manager', 3),
    ('Linda', 'Davis', '2020-02-14', 'Mechanic', 4),
    ('William', 'Lopez', '2019-07-19', 'Technician', 5);

INSERT INTO Department (DepartmentName)
VALUES 
    ('Flight Operations'),
    ('Maintenance'),
    ('Sales'),
    ('Customer Service'),
    ('IT Support');

INSERT INTO Sale (CustomerID, AircraftID, SaleDate, EmployeeID)
VALUES 
    (1, 1, '2022-05-15', 1),
    (2, 2, '2023-02-10', 2),
    (3, 3, '2021-07-22', 3),
    (4, 4, '2020-12-18', 4),
    (5, 5, '2023-09-30', 5);

INSERT INTO Service (ServiceName, Cost)
VALUES 
    ('Engine Overhaul', 50000.00),
    ('Avionics Upgrade', 75000.00),
    ('Landing Gear Inspection', 30000.00),
    ('Cabin Refurbishment', 100000.00),
    ('Paint Touch-Up', 15000.00);

INSERT INTO Aircraft_Service (AircraftID, ServiceID, ServiceDate)
VALUES 
    (1, 1, '2023-01-05'),
    (2, 2, '2022-11-15'),
    (3, 3, '2023-06-20'),
    (4, 4, '2021-03-30'),
    (5, 5, '2023-08-25');

INSERT INTO ServiceHistory (AircraftID, ServiceID, EmployeeID, ServiceDate, Notes)
VALUES 
    (1, 1, 1, '2023-02-10', 'Engine overhaul completed successfully.'),
    (2, 2, 2, '2022-12-05', 'Avionics upgrade installed.'),
    (3, 3, 3, '2023-07-15', 'Landing gear inspected with no issues.'),
    (4, 4, 4, '2021-04-10', 'Cabin refurbishment finished.'),
    (5, 5, 5, '2023-09-01', 'Minor paint touch-up completed.');
