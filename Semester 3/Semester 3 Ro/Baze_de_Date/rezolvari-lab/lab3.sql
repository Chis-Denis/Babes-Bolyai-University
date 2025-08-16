GO
USE Facultatea
GO


CREATE PROCEDURE MAIN (@v INT)
AS BEGIN
IF @v < 0 OR @v > 5
	RAISERROR('Version doesn''t exist.',11,1)

ELSE
BEGIN
	DECLARE @currentV INT
	SET @currentV =(SELECT NrV FROM Versiune);
	
	IF @currentV=@v
		PRINT 'DONE!'
	ELSE
	BEGIN
		DECLARE @x VARCHAR(10)
		if @v > @currentV --la versiune superioara
			while @v>@currentV
			BEGIN
				SET @currentV= @currentV+1
				SET @x = (SELECT CAST(@currentV as varchar(10)))

				DECLARE @p VARCHAR(10)
				SET @p=CONCAT('P',@x)
				PRINT(@p)
				EXEC @p;
			END	
		ELSE --la versiune inferioara
			while @v < @currentV AND @currentV >= 0
				BEGIN
					SET @x = (SELECT CAST(@currentV as varchar(10)))

					SET @p=CONCAT('iP',@x)
					
					PRINT(@p)
					EXEC @p;
					SET @currentV=@currentV -1
				END	
		UPDATE Versiune SET NrV=@v;
		END
	END
	
END

DROP PROCEDURE MAIN

EXEC MAIN 0


SELECT NrV FROM Versiune
