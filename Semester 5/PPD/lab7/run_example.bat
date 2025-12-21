@echo off
REM Run the polynomial_mpi example
set MPI_BIN=C:\Program Files\Microsoft MPI\Bin

"%MPI_BIN%\mpiexec.exe" -n 4 .\polynomial_mpi.exe example

pause

