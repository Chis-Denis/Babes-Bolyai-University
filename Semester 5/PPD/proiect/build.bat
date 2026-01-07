@echo off
REM Build script for Windows

if not exist build mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release

echo Build complete! Executables are in the build/ directory.

