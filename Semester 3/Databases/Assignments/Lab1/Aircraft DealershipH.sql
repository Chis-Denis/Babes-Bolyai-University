-- Table for storing customers
CREATE TABLE Customer (
    CustomerID INT PRIMARY KEY IDENTITY,
    FirstName NVARCHAR(50),
    LastName NVARCHAR(50),
    PhoneNumber NVARCHAR(15),
    Email NVARCHAR(100)
);

-- Table for storing manufacturers
CREATE TABLE Manufacturer (
    ManufacturerID INT PRIMARY KEY IDENTITY,
    Name NVARCHAR(100),
    Country NVARCHAR(50)
);

-- Table for storing aircraft models
CREATE TABLE AircraftModel (
    ModelID INT PRIMARY KEY IDENTITY,
    ModelName NVARCHAR(100),
    ManufacturerID INT,
    YearOfProduction INT,
    FOREIGN KEY (ManufacturerID) REFERENCES Manufacturer(ManufacturerID)
);

-- Table for storing aircraft details
CREATE TABLE Aircraft (
    AircraftID INT PRIMARY KEY IDENTITY,
    TailNumber NVARCHAR(10) UNIQUE,
    ModelID INT,
    PurchaseDate DATE,
    Price DECIMAL(18, 2),
    FOREIGN KEY (ModelID) REFERENCES AircraftModel(ModelID)
);

-- Table for storing employees
CREATE TABLE Employee (
    EmployeeID INT PRIMARY KEY IDENTITY,
    FirstName NVARCHAR(50),
    LastName NVARCHAR(50),
    HireDate DATE,
    Position NVARCHAR(50),
    DepartmentID INT
);

-- Table for departments
CREATE TABLE Department (
    DepartmentID INT PRIMARY KEY IDENTITY,
    DepartmentName NVARCHAR(50)
);

-- Table for tracking sales
CREATE TABLE Sale (
    SaleID INT PRIMARY KEY IDENTITY,
    CustomerID INT,
    AircraftID INT,
    SaleDate DATE,
    EmployeeID INT,
    FOREIGN KEY (CustomerID) REFERENCES Customer(CustomerID),
    FOREIGN KEY (AircraftID) REFERENCES Aircraft(AircraftID),
    FOREIGN KEY (EmployeeID) REFERENCES Employee(EmployeeID)
);

-- Table for storing services (maintanance)
CREATE TABLE Service (
    ServiceID INT PRIMARY KEY IDENTITY,
    ServiceName NVARCHAR(100),
    Cost DECIMAL(18, 2)
);

-- What type of maintancence between Aircraft and Service
CREATE TABLE Aircraft_Service (
    AircraftID INT,
    ServiceID INT,
    ServiceDate DATE,
    PRIMARY KEY (AircraftID, ServiceID),
    FOREIGN KEY (AircraftID) REFERENCES Aircraft(AircraftID),
    FOREIGN KEY (ServiceID) REFERENCES Service(ServiceID)
);

-- Table for storing service history
CREATE TABLE ServiceHistory (
    ServiceHistoryID INT PRIMARY KEY IDENTITY,
    AircraftID INT,
    ServiceID INT,
    EmployeeID INT,
    ServiceDate DATE,
    Notes NVARCHAR(500),
    FOREIGN KEY (AircraftID) REFERENCES Aircraft(AircraftID),
    FOREIGN KEY (ServiceID) REFERENCES Service(ServiceID),
    FOREIGN KEY (EmployeeID) REFERENCES Employee(EmployeeID)
);
