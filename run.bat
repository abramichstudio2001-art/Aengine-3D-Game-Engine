@echo off
setlocal enabledelayedexpansion

echo ===================================================
echo   Aengine 3D Game Engine - Auto Build & Run Script
echo ===================================================

:: Step 1: Check prerequisites
echo [1/4] Checking prerequisites...
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] 'cmake' was not found in PATH!
    echo Please install CMake from https://cmake.org/download/ and add it to system PATH.
    echo Exiting...
    pause
    exit /b 1
)

echo [OK] CMake is available.

:: Step 2: Configure project and download dependencies via FetchContent
echo [2/4] Configuring project and downloading dependencies (GLFW, GLM, ImGui)...
cmake -B build
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    echo Please check compiler installation (MSVC, MinGW, or GCC/Clang).
    pause
    exit /b %ERRORLEVEL%
)

:: Step 3: Compile project
echo [3/4] Compiling Aengine 3D...
cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed! Check compiler output above for details.
    pause
    exit /b %ERRORLEVEL%
)

:: Step 4: Locate and Launch Executable
echo [4/4] Locating executable and starting engine...

if exist build\Release\Aengine3D.exe (
    pushd build\Release
    echo Starting Aengine3D.exe...
    Aengine3D.exe %*
    popd
) else if exist build\Debug\Aengine3D.exe (
    pushd build\Debug
    echo Starting Aengine3D.exe...
    Aengine3D.exe %*
    popd
) else if exist build\Aengine3D.exe (
    pushd build
    echo Starting Aengine3D.exe...
    Aengine3D.exe %*
    popd
) else (
    echo [ERROR] Executable Aengine3D.exe was not found in build directory.
    pause
    exit /b 1
)

endlocal
