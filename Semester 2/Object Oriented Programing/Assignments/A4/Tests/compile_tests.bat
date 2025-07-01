@echo off
REM Compile tests using g++ WITH COVERAGE - Multi-step with Response File (Relative Paths v3 + Goto)

REM Define directories using backslashes for cmd.exe compatibility
set SRC_DIR=.
set TEST_DIR=%SRC_DIR%\Tests
set OBJ_DIR=%SRC_DIR%\obj
set GTEST_DIR=%TEST_DIR%\googletest-main\googletest
set OUT_EXE=%SRC_DIR%\run_tests.exe
set LINK_ARGS_FILE=%OBJ_DIR%\link_args.txt

set CPP=g++
REM Note: Keep --coverage in CPP_FLAGS for both compile and link steps
set CPP_FLAGS=-std=c++17 -Wall -Wextra -pthread --coverage
REM Use forward slashes in include flags for g++
set INCLUDE_FLAGS=-I%GTEST_DIR%/include -I%GTEST_DIR% -I%SRC_DIR%

echo Cleaning previous build...
if exist %OBJ_DIR% rd /s /q %OBJ_DIR%
if exist %OUT_EXE% del %OUT_EXE%
if exist *.gcda del *.gcda
if exist *.gcno del *.gcno

md %OBJ_DIR% || exit /b 1

echo Compiling tests with coverage enabled...

REM --- Compilation steps (use forward slashes for g++) ---
%CPP% %CPP_FLAGS% %INCLUDE_FLAGS% -c %SRC_DIR%/Domain/trench.cpp -o %OBJ_DIR%/trench.o
if %errorlevel% neq 0 ( echo Failed on trench.cpp & goto EndScript )
%CPP% %CPP_FLAGS% %INCLUDE_FLAGS% -c %SRC_DIR%/Repository/trenchCoatRepository.cpp -o %OBJ_DIR%/trenchCoatRepository.o
if %errorlevel% neq 0 ( echo Failed on trenchCoatRepository.cpp & goto EndScript )
%CPP% %CPP_FLAGS% %INCLUDE_FLAGS% -c %SRC_DIR%/Controller/trenchCoatController.cpp -o %OBJ_DIR%/trenchCoatController.o
if %errorlevel% neq 0 ( echo Failed on trenchCoatController.cpp & goto EndScript )
%CPP% %CPP_FLAGS% %INCLUDE_FLAGS% -c %GTEST_DIR%/src/gtest-all.cc -o %OBJ_DIR%/gtest-all.o
if %errorlevel% neq 0 ( echo Failed on gtest-all.cc & goto EndScript )
%CPP% %CPP_FLAGS% %INCLUDE_FLAGS% -c %TEST_DIR%/test_main.cpp -o %OBJ_DIR%/test_main.o
if %errorlevel% neq 0 ( echo Failed on test_main.cpp & goto EndScript )
%CPP% %CPP_FLAGS% %INCLUDE_FLAGS% -c %TEST_DIR%/controller_test.cpp -o %OBJ_DIR%/controller_test.o
if %errorlevel% neq 0 ( echo Failed on controller_test.cpp & goto EndScript )
%CPP% %CPP_FLAGS% %INCLUDE_FLAGS% -c %TEST_DIR%/Domain/test_dynamicVector.cpp -o %OBJ_DIR%/test_dynamicVector.o
if %errorlevel% neq 0 ( echo Failed on test_dynamicVector.cpp & goto EndScript )

echo Linking using response file with relative paths...

REM --- Create the response file for linking ---
REM Use RELATIVE paths with FORWARD slashes directly
(
    echo obj/trench.o
    echo obj/trenchCoatRepository.o
    echo obj/trenchCoatController.o
    echo obj/gtest-all.o
    echo obj/test_main.o
    echo obj/controller_test.o
    echo obj/test_dynamicVector.o
) > %LINK_ARGS_FILE%

if %errorlevel% neq 0 ( echo Failed to create link arguments file & goto EndScript )

REM --- Link using the response file ---
REM Pass flags like --coverage directly, put only input files in the response file
%CPP% %CPP_FLAGS% -o %OUT_EXE% @%LINK_ARGS_FILE%

REM --- Check link status using goto ---
if %errorlevel% neq 0 goto LinkFail

echo Compilation successful.
if exist %LINK_ARGS_FILE% del %LINK_ARGS_FILE% 
echo You can run tests using: .\run_tests.exe
echo Then generate coverage report (requires lcov).
REM Optional: Clean up object files after successful build
REM rd /s /q %OBJ_DIR%
goto EndScript

:LinkFail
echo Linking failed!
if exist %LINK_ARGS_FILE% del %LINK_ARGS_FILE%
REM Keep the errorlevel from the failed link step

:EndScript 