@echo off
setlocal enabledelayedexpansion

:: Set build directory
set BUILD_DIR=build

:: Check for command line arguments
if "%1"=="" goto help
if "%1"=="build" goto build
if "%1"=="clean" goto clean
if "%1"=="run" goto run
if "%1"=="test" goto test
if "%1"=="test-eval" goto test-eval
if "%1"=="test-search" goto test-search
if "%1"=="test-all" goto test-all
if "%1"=="rebuild" goto rebuild
if "%1"=="help" goto help

echo Unknown command: %1
goto help

:build
echo Building Chess-E...
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%
cmake ..
cmake --build .
cd ..
echo Build complete.
goto end

:clean
echo Cleaning build directory...
if exist %BUILD_DIR% rmdir /s /q %BUILD_DIR%
echo Clean complete.
goto end

:run
echo Running Chess-E engine...
if not exist %BUILD_DIR%\chess_engine.exe (
    echo Chess engine not built yet. Building first...
    call :build
)
cd %BUILD_DIR%
.\chess_engine.exe
cd ..
goto end

:test
echo Running basic test suite...
if not exist %BUILD_DIR%\test_engine.exe (
    echo Test engine not built yet. Building first...
    call :build
)
cd %BUILD_DIR%
.\test_engine.exe
cd ..
goto end

:test-eval
echo Running evaluation tests...
if not exist %BUILD_DIR%\test_evaluation.exe (
    echo Evaluation tests not built yet. Building first...
    call :build
)
cd %BUILD_DIR%
.\test_evaluation.exe
cd ..
goto end

:test-search
echo Running search algorithm tests...
if not exist %BUILD_DIR%\test_search.exe (
    echo Search tests not built yet. Building first...
    call :build
)
cd %BUILD_DIR%
.\test_search.exe
cd ..
goto end

:test-all
echo Running all tests...
call :test
call :test-eval
call :test-search
goto end

:rebuild
call :clean
call :build
goto end

:help
echo Chess-E Windows Build Script
echo ===========================
echo.
echo Usage: build.bat [command]
echo.
echo Commands:
echo   build       - Build the chess engine
echo   clean       - Remove build artifacts
echo   run         - Run the chess engine
echo   test        - Run the basic test suite
echo   test-eval   - Run evaluation tests
echo   test-search - Run search algorithm tests
echo   test-all    - Run all tests
echo   rebuild     - Rebuild from scratch
echo   help        - Show this help message
echo.
echo Example: build.bat run

:end
endlocal 