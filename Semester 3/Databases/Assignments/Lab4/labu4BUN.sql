-- ============================================
-- 1. Drop Existing Tables and Cleanup (Optional)
-- ============================================
DROP TABLE IF EXISTS TestRunViews, TestRunTables, TestRuns, TestViews, ViewsForTesting,
                     TestTables, TablesForTesting, Tests;
DROP TABLE IF EXISTS OrderDetails, Orders, Customers;
GO

-- ============================================
-- 2. Create Core Tables and Structure
-- ============================================

-- Tests table
CREATE TABLE Tests (
    TestID INT IDENTITY PRIMARY KEY,
    TestName NVARCHAR(100) NOT NULL
);

-- TablesForTesting: holds table names
CREATE TABLE TablesForTesting (
    TableID INT IDENTITY PRIMARY KEY,
    TableName NVARCHAR(100) NOT NULL
);

-- TestTables: junction table between Tests and Tables
CREATE TABLE TestTables (
    TestID INT NOT NULL,
    TableID INT NOT NULL,
    Position INT NOT NULL,
    NoOfRows INT NOT NULL, -- Number of rows to insert
    PRIMARY KEY (TestID, TableID),
    FOREIGN KEY (TestID) REFERENCES Tests(TestID),
    FOREIGN KEY (TableID) REFERENCES TablesForTesting(TableID)
);

-- ViewsForTesting: holds view names
CREATE TABLE ViewsForTesting (
    ViewID INT IDENTITY PRIMARY KEY,
    ViewName NVARCHAR(100) NOT NULL
);

-- TestViews: junction table between Tests and Views
CREATE TABLE TestViews (
    TestID INT NOT NULL,
    ViewID INT NOT NULL,
    PRIMARY KEY (TestID, ViewID),
    FOREIGN KEY (TestID) REFERENCES Tests(TestID),
    FOREIGN KEY (ViewID) REFERENCES ViewsForTesting(ViewID)
);

-- TestRuns: tracks different runs of a test
CREATE TABLE TestRuns (
    TestRunID INT IDENTITY PRIMARY KEY,
    TestID INT NOT NULL,
    RunDate DATETIME NOT NULL,
    FOREIGN KEY (TestID) REFERENCES Tests(TestID)
);

-- TestRunTables: performance data for table INSERT operations
CREATE TABLE TestRunTables (
    TestRunID INT NOT NULL,
    TableID INT NOT NULL,
    InsertTimeMs INT NOT NULL,
    PRIMARY KEY (TestRunID, TableID),
    FOREIGN KEY (TestRunID) REFERENCES TestRuns(TestRunID),
    FOREIGN KEY (TableID) REFERENCES TablesForTesting(TableID)
);

-- TestRunViews: performance data for view SELECT operations
CREATE TABLE TestRunViews (
    TestRunID INT NOT NULL,
    ViewID INT NOT NULL,
    SelectTimeMs INT NOT NULL,
    PRIMARY KEY (TestRunID, ViewID),
    FOREIGN KEY (TestRunID) REFERENCES TestRuns(TestRunID),
    FOREIGN KEY (ViewID) REFERENCES ViewsForTesting(ViewID)
);

-- ============================================
-- 3. Create Tables for Testing Data
-- ============================================

-- Table 1: Single-column primary key, no foreign keys
CREATE TABLE Customers (
    CustomerID INT IDENTITY PRIMARY KEY,
    FirstName NVARCHAR(50),
    LastName NVARCHAR(50)
);

-- Table 2: Single-column primary key, at least one foreign key
CREATE TABLE Orders (
    OrderID INT IDENTITY PRIMARY KEY,
    CustomerID INT NOT NULL,
    OrderDate DATE,
    FOREIGN KEY (CustomerID) REFERENCES Customers(CustomerID)
);

-- Table 3: Multi-column primary key
CREATE TABLE OrderDetails (
    OrderID INT NOT NULL,
    ProductID INT NOT NULL,
    Quantity INT,
    PRIMARY KEY (OrderID, ProductID),
    FOREIGN KEY (OrderID) REFERENCES Orders(OrderID)
);

-- ============================================
-- 4. Create Views for Testing
-- ============================================

-- View 1: SELECT on one table
CREATE VIEW v_Customers AS
SELECT CustomerID, FirstName, LastName
FROM Customers;

-- View 2: SELECT with JOIN on two tables
CREATE VIEW v_Orders AS
SELECT O.OrderID, C.FirstName, C.LastName, O.OrderDate
FROM Orders O
JOIN Customers C ON O.CustomerID = C.CustomerID;

-- View 3: SELECT with GROUP BY and JOIN
CREATE VIEW v_OrderSummary AS
SELECT O.CustomerID, COUNT(OD.ProductID) AS TotalProducts, SUM(OD.Quantity) AS TotalQuantity
FROM Orders O
JOIN OrderDetails OD ON O.OrderID = OD.OrderID
GROUP BY O.CustomerID;

-- ============================================
-- 5. Stored Procedure: Run Tests
-- ============================================

CREATE OR ALTER PROCEDURE sp_RunTest
@TestID INT
AS
BEGIN
    -- Prevents additional result sets from being returned
    SET NOCOUNT ON;

    -- Declare local variables
    DECLARE @RunDate DATETIME = GETDATE();  -- Current date and time for the test run
    DECLARE @TestRunID INT;  -- Holds the ID of the test run
    DECLARE @TableName NVARCHAR(100), @NoOfRows INT, @SQL NVARCHAR(MAX);  -- Variables for table name, number of rows, and dynamic SQL
    DECLARE @ViewName NVARCHAR(100);  -- Variable for the view name
    DECLARE @StartTime DATETIME, @EndTime DATETIME;  -- Variables to track start and end time for performance measurement


    -- Start a new test run
    -- Insert a new test run entry in the TestRuns table
    INSERT INTO TestRuns (TestID, RunDate) VALUES (@TestID, @RunDate);
    
    -- Capture the ID of the newly inserted test run
    SET @TestRunID = SCOPE_IDENTITY();


    -- Deleting and Inserting Table Data
    -- Declare a cursor to fetch table names and row counts from the TestTables and TablesForTesting tables
    DECLARE TableCursor CURSOR FOR
    SELECT TT.TableID, TT.NoOfRows
    FROM TestTables TT
    WHERE TT.TestID = @TestID  -- Only fetch tables associated with the current test
    ORDER BY TT.Position;  -- Order tables based on their position for proper processing order

    OPEN TableCursor;  -- Open the cursor to begin fetching data
    FETCH NEXT FROM TableCursor INTO @TableName, @NoOfRows;  -- Fetch the first table and number of rows

    -- Loop through each table and perform deletion and insertion of dummy data
    WHILE @@FETCH_STATUS = 0
    BEGIN
        -- Delete existing data from the table
        SET @SQL = N'DELETE FROM ' + QUOTENAME(@TableName);  -- Build the DELETE SQL command
        EXEC sp_executesql @SQL;  -- Execute the dynamic DELETE command

        -- Record the start time for the insert operation
        SET @StartTime = GETDATE();

        -- Insert dummy data into the table (using system view master.dbo.spt_values for quick row generation)
        SET @SQL = N'INSERT INTO ' + QUOTENAME(@TableName) + 
            ' SELECT TOP (' + CAST(@NoOfRows AS NVARCHAR) + ') 1, 1, 1 FROM master.dbo.spt_values';
        EXEC sp_executesql @SQL;  -- Execute the dynamic INSERT command

        -- Record the end time for the insert operation
        SET @EndTime = GETDATE();

        -- Insert the performance measurement (time taken for insert) into the TestRunTables table
        INSERT INTO TestRunTables (TestRunID, TableID, InsertTimeMs)
        SELECT @TestRunID, TFT.TableID, DATEDIFF(MILLISECOND, @StartTime, @EndTime)
        FROM TablesForTesting TFT WHERE TFT.TableName = @TableName;

        -- Fetch the next table and its row count
        FETCH NEXT FROM TableCursor INTO @TableName, @NoOfRows;
    END;

    -- Close and deallocate the TableCursor
    CLOSE TableCursor;
    DEALLOCATE TableCursor;


    -- Measuring View Performance
    -- Declare a cursor to fetch view names from the TestViews and ViewsForTesting tables
    DECLARE ViewCursor CURSOR FOR
    SELECT ViewID
    FROM TestViews
    WHERE TestID = @TestID;

    OPEN ViewCursor;  -- Open the cursor to begin fetching data
    FETCH NEXT FROM ViewCursor INTO @ViewName;  -- Fetch the first view name

    -- Loop through each view and measure the performance of a SELECT query on the view
    WHILE @@FETCH_STATUS = 0
    BEGIN
        -- Record the start time for the view query
        SET @StartTime = GETDATE();

        -- Build the SELECT SQL statement to execute the query on the view
        SET @SQL = N'SELECT * FROM ' + QUOTENAME(@ViewName);
        EXEC sp_executesql @SQL;  -- Execute the dynamic SELECT command

        -- Record the end time for the view query execution
        SET @EndTime = GETDATE();

        -- Insert the performance measurement (time taken for SELECT) into the TestRunViews table
        INSERT INTO TestRunViews (TestRunID, ViewID, SelectTimeMs)
        SELECT @TestRunID, VFT.ViewID, DATEDIFF(MILLISECOND, @StartTime, @EndTime)
        FROM ViewsForTesting VFT WHERE VFT.ViewName = @ViewName;

        -- Fetch the next view name
        FETCH NEXT FROM ViewCursor INTO @ViewName;
    END;

    -- Close and deallocate the ViewCursor
    CLOSE ViewCursor;
    DEALLOCATE ViewCursor;

END;
GO



-- 6. Insert Test Configuration and Run Test

-- Insert test configuration
INSERT INTO Tests (TestName) VALUES ('Performance Test');
DECLARE @TestID INT = SCOPE_IDENTITY();

-- Register tables
INSERT INTO TablesForTesting (TableName) VALUES ('Customers'), ('Orders'), ('OrderDetails');
INSERT INTO TestTables (TestID, TableID, Position, NoOfRows)
VALUES (@TestID, 1, 1, 10), (@TestID, 2, 2, 5), (@TestID, 3, 3, 20);

-- Register views
INSERT INTO ViewsForTesting (ViewName) VALUES ('v_Customers'), ('v_Orders'), ('v_OrderSummary');
INSERT INTO TestViews (TestID, ViewID) VALUES (@TestID, 1), (@TestID, 2), (@TestID, 3);

-- Run the test
EXEC sp_RunTest @TestID;

-- Check results
SELECT * FROM TestRunTables;
SELECT * FROM TestRunViews;
