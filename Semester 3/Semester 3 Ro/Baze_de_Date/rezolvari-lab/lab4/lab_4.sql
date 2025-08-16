USE Spotify
GO

--se realizeaza inserarea in tabelul Tables
--tabelele pe care se vor executa teste sunt , in ordine : Artisti , Cantece , Contracte
EXEC insertIntoTables 'Artisti';
EXEC insertIntoTables 'Cantece';
EXEC insertIntoTables 'Contracte';
SELECT * FROM Tables;

--se realizeaza inserarea in tabelul Views
--numele view-ului va fi generat automat
--se va da ca parametru numarul de view-uri care se vor a fi create 
EXEC insertIntoViews 3;
SELECT * FROM Views;

--se realizeaza inserarea automata in tabelul Tests
--inserarea se va realiza prin completarea de nume de teste sugestive
DECLARE @nrTables INT,@nrViews INT;
SELECT @nrTables = MAX(TableId) FROM Tables
SELECT @nrViews = MAX(ViewId) FROM Views
EXEC insertIntoTests @nrTables,@nrViews;
SELECT * FROM Tests;

--se realizeaza completarea tabelului TestViews
EXEC insertIntoTestViews;
SELECT * FROM TestViews;

--se realizeaza completarea tabelului TestTables
EXEC insertIntoTestTables;
SELECT * FROM TestTables;

--se insereaza in tabelul colaborari
EXEC insertIntoColaborari 100;
SELECT * FROM Colaborari;
SELECT * FROM Cantece

EXEC execTest 'Artisti_V1_10' 
EXEC execTest 'Artisti_V2_100' 
EXEC execTest 'Artisti_V3_1000'

EXEC execTest 'Cantece_V1_10'
EXEC execTest 'Cantece_V2_100'
EXEC execTest 'Cantece_V3_1000'

EXEC execTest 'Contracte_V1_10'
EXEC execTest 'Contracte_V2_100'
EXEC execTest 'Contracte_V3_1000'

SELECT * FROM TestRuns
SELECT * FROM TestRunTables
SELECT * FROM TestRunViews


--reset IDENTITY field
--DBCC CHECKIDENT ('TableName', RESEED, 0)
