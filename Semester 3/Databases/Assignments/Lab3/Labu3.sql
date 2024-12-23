-- Create the SchemaVersion table
IF NOT EXISTS (SELECT * FROM sysobjects WHERE name = 'SchemaVersion' AND xtype = 'U')
BEGIN
    CREATE TABLE SchemaVersion (
        VersionNumber INT PRIMARY KEY,
    );

    -- Initialize the schema version
    INSERT INTO SchemaVersion (VersionNumber) VALUES (0);
END;
GO

-- Modify column type (a)
CREATE OR ALTER PROCEDURE sp_ModifyColumnType_Customer_PhoneNumber_ToV1
AS
BEGIN
    -- Change PhoneNumber column type to NVARCHAR(50)
    ALTER TABLE Customer ALTER COLUMN PhoneNumber NVARCHAR(20);
END;
GO

CREATE OR ALTER PROCEDURE sp_RevertColumnType_Customer_PhoneNumber_ToV0
AS
BEGIN
    -- Revert PhoneNumber column type to NVARCHAR(15)
    ALTER TABLE Customer ALTER COLUMN PhoneNumber NVARCHAR(15);
END;
GO

-- Add/Remove column (b)
CREATE OR ALTER PROCEDURE sp_AddColumn_Customer_MiddleName_ToV2
AS
BEGIN
    -- Add a new column for MiddleName with a size of NVARCHAR(50)
    ALTER TABLE Customer ADD MiddleName NVARCHAR(50);
END;
GO

CREATE OR ALTER PROCEDURE sp_RemoveColumn_Customer_MiddleName_ToV1
AS
BEGIN
    -- Remove the MiddleName column
    ALTER TABLE Customer DROP COLUMN MiddleName;
END;
GO

-- Add/Remove DEFAULT constraint (c)
CREATE OR ALTER PROCEDURE sp_AddDefault_Manufacturer_Country_ToV3
AS
BEGIN
    -- Add a DEFAULT constraint for the Country column
    ALTER TABLE Manufacturer ADD CONSTRAINT DF_Manufacturer_Country DEFAULT ('Unknown') FOR Country;
END;
GO

CREATE OR ALTER PROCEDURE sp_RemoveDefault_Manufacturer_Country_ToV2
AS
BEGIN
    -- Remove the DEFAULT constraint for the Country column
    ALTER TABLE Manufacturer DROP CONSTRAINT DF_Manufacturer_Country;
END;
GO

-- Add/Remove Primary Key (d)
CREATE OR ALTER PROCEDURE sp_AddPrimaryKey_AircraftService_ToV4
AS
BEGIN
    -- Add a primary key to the Aircraft_Service table
    ALTER TABLE Aircraft_Service ADD CONSTRAINT PK_Aircraft_Service PRIMARY KEY (ServiceID);
END;
GO

CREATE OR ALTER PROCEDURE sp_RemovePrimaryKey_AircraftService_ToV3
AS
BEGIN
    -- Drop the primary key from the Aircraft_Service table
    ALTER TABLE Aircraft_Service DROP CONSTRAINT PK_Aircraft_Service;
END;
GO

-- Add/Remove Candidate Key (e)
CREATE OR ALTER PROCEDURE sp_AddCandidateKey_Customer_Email_ToV5
AS
BEGIN
    -- Add a unique constraint to the Email column
    ALTER TABLE Customer ADD CONSTRAINT UQ_Customer_Email UNIQUE (Email);
END;
GO

CREATE OR ALTER PROCEDURE sp_RemoveCandidateKey_Customer_Email_ToV4
AS
BEGIN
    -- Drop the unique constraint from the Email column
    ALTER TABLE Customer DROP CONSTRAINT UQ_Customer_Email;
END;
GO

-- Add/Remove Foreign Key (f)
CREATE OR ALTER PROCEDURE sp_AddForeignKey_Employee_Department_ToV6
AS
BEGIN
    -- Add a foreign key constraint linking Employee.DepartmentID to Department.DepartmentID
    ALTER TABLE Employee ADD CONSTRAINT FK_Employee_Department FOREIGN KEY (DepartmentID) REFERENCES Department(DepartmentID);
END;
GO

CREATE OR ALTER PROCEDURE sp_RemoveForeignKey_Employee_Department_ToV5
AS
BEGIN
    -- Remove the foreign key constraint
    ALTER TABLE Employee DROP CONSTRAINT FK_Employee_Department;
END;
GO

-- Create/Drop Table (g)
CREATE OR ALTER PROCEDURE sp_CreateTable_Pilot_ToV7
AS
BEGIN
    CREATE TABLE Pilot (
        PilotID INT PRIMARY KEY IDENTITY,
        FirstName NVARCHAR(50),
        LastName NVARCHAR(50),
        LicenseNumber NVARCHAR(20) UNIQUE
    );
END;
GO

CREATE OR ALTER PROCEDURE sp_DropTable_Pilot_ToV6
AS
BEGIN
    DROP TABLE Pilot;
END;
GO

-- Switch schema version
CREATE OR ALTER PROCEDURE sp_SwitchSchemaVersion (@TargetVersion INT)
AS
BEGIN
    DECLARE @CurrentVersion INT;
    SELECT @CurrentVersion = VersionNumber FROM SchemaVersion;

    -- If the target version is the same as the current version, no action is needed
    IF @TargetVersion = @CurrentVersion RETURN;

    -- Move forward to higher versions
    WHILE @TargetVersion > @CurrentVersion
    BEGIN
        SET @CurrentVersion = @CurrentVersion + 1;

        -- Call the corresponding procedure for each version increment
        IF @CurrentVersion = 1
            EXEC sp_ModifyColumnType_Customer_PhoneNumber_ToV1;
        IF @CurrentVersion = 2
            EXEC sp_AddColumn_Customer_MiddleName_ToV2;
        IF @CurrentVersion = 3
            EXEC sp_AddDefault_Manufacturer_Country_ToV3;
        IF @CurrentVersion = 4
            EXEC sp_AddPrimaryKey_AircraftService_ToV4;
        IF @CurrentVersion = 5
            EXEC sp_AddCandidateKey_Customer_Email_ToV5;
        IF @CurrentVersion = 6
            EXEC sp_AddForeignKey_Employee_Department_ToV6;
        IF @CurrentVersion = 7
            EXEC sp_CreateTable_Pilot_ToV7;
    END;

    -- Move backward to lower versions
    WHILE @TargetVersion < @CurrentVersion
    BEGIN
        -- Call the corresponding procedure for each version decrement
        IF @CurrentVersion = 7
            EXEC sp_DropTable_Pilot_ToV6;
        IF @CurrentVersion = 6
            EXEC sp_RemoveForeignKey_Employee_Department_ToV5;
        IF @CurrentVersion = 5
            EXEC sp_RemoveCandidateKey_Customer_Email_ToV4;
        IF @CurrentVersion = 4
            EXEC sp_RemovePrimaryKey_AircraftService_ToV3;
        IF @CurrentVersion = 3
            EXEC sp_RemoveDefault_Manufacturer_Country_ToV2;
        IF @CurrentVersion = 2
            EXEC sp_RemoveColumn_Customer_MiddleName_ToV1;
        IF @CurrentVersion = 1
            EXEC sp_RevertColumnType_Customer_PhoneNumber_ToV0;

        SET @CurrentVersion = @CurrentVersion - 1;
    END;

    -- Update the SchemaVersion table
    UPDATE SchemaVersion SET VersionNumber = @TargetVersion;
END;
GO

select * from SchemaVersion
EXEC sp_SwitchSchemaVersion @TargetVersion = 1;

select * from Customer
delete from Customer
where CustomerID = 2

DELETE FROM SchemaVersion
WHERE VersionNumber = 4;

SELECT 
    OBJECT_NAME(OBJECT_ID) AS TableName,
    name AS ConstraintName,
    type_desc AS ConstraintType
FROM sys.objects
WHERE type_desc LIKE '%CONSTRAINT'
    AND parent_object_id = OBJECT_ID('Aircraft_Service');
