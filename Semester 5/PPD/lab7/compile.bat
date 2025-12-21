@echo off
REM Compile MPI programs using MS-MPI
REM This script compiles without mpicc wrapper

set MPI_INCLUDE=C:\Program Files (x86)\Microsoft SDKs\MPI\Include
set MPI_LIB=C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x64
set MPI_BIN=C:\Program Files\Microsoft MPI\Bin

REM Check for available compilers
where gcc >nul 2>&1
if %errorlevel% equ 0 (
    echo Using GCC compiler...
    gcc -Wall -O2 -std=c99 -I"%MPI_INCLUDE%" -L"%MPI_LIB%" -o polynomial_mpi.exe polynomial_mpi.c -lmsmpi
    gcc -Wall -O2 -std=c99 -I"%MPI_INCLUDE%" -L"%MPI_LIB%" -o bigint_mpi.exe bigint_mpi.c -lmsmpi
    gcc -Wall -O2 -std=c99 -I"%MPI_INCLUDE%" -L"%MPI_LIB%" -o compare.exe compare.c -lmsmpi
    goto :done
)

where cl >nul 2>&1
if %errorlevel% equ 0 (
    echo Using Visual Studio compiler...
    cl /W3 /O2 /std:c99 /I"%MPI_INCLUDE%" /Fe:polynomial_mpi.exe polynomial_mpi.c "%MPI_LIB%\msmpi.lib"
    cl /W3 /O2 /std:c99 /I"%MPI_INCLUDE%" /Fe:bigint_mpi.exe bigint_mpi.c "%MPI_LIB%\msmpi.lib"
    cl /W3 /O2 /std:c99 /I"%MPI_INCLUDE%" /Fe:compare.exe compare.c "%MPI_LIB%\msmpi.lib"
    goto :done
)

echo ERROR: No C compiler found!
echo Please install MinGW (gcc) or Visual Studio (cl)
echo Or use WSL: wsl sudo apt-get install libopenmpi-dev openmpi-bin
pause
exit /b 1

:done
echo.
echo Compilation complete!
echo.
echo To run:
echo   "%MPI_BIN%\mpiexec.exe" -n 4 .\polynomial_mpi.exe example
pause

