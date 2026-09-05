@echo off
setlocal enabledelayedexpansion

echo ===================================================
echo   Aengine 3D Game Engine - Auto Build & Run Script
echo ===================================================

:: Step 1: Detect CMake and Compiler Environment (MSVC / MinGW / MSYS2)
echo [1/4] Checking CMake and environment compiler toolchain...

set "CMAKE_GEN="
set "MSYS_FOUND=0"

if exist "C:\msys64\ucrt64\bin\cmake.exe" (
    set "PATH=C:\msys64\ucrt64\bin;!PATH!"
    set "CMAKE_GEN=-G "MinGW Makefiles""
    set "MSYS_FOUND=1"
) else if exist "C:\msys64\mingw64\bin\cmake.exe" (
    set "PATH=C:\msys64\mingw64\bin;!PATH!"
    set "CMAKE_GEN=-G "MinGW Makefiles""
    set "MSYS_FOUND=1"
) else if exist "C:\msys64\clang64\bin\cmake.exe" (
    set "PATH=C:\msys64\clang64\bin;!PATH!"
    set "CMAKE_GEN=-G "MinGW Makefiles""
    set "MSYS_FOUND=1"
) else if exist "C:\Program Files\CMake\bin\cmake.exe" (
    set "PATH=C:\Program Files\CMake\bin;!PATH!"
)

where gcc >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    if "!CMAKE_GEN!"=="" set "CMAKE_GEN=-G "MinGW Makefiles""
)

where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] 'cmake' was not found in PATH or standard MSYS2/CMake installation paths!
    echo Please install CMake (https://cmake.org/download/) or MSYS2 (https://www.msys2.org/).
    pause
    exit /b 1
)

echo [OK] CMake environment initialized.

:: Step 2: Configure project and download dependencies via FetchContent
echo [2/4] Configuring project and downloading dependencies (GLFW, GLM, ImGui)...
if defined CMAKE_GEN (
    cmake -B build !CMAKE_GEN!
) else (
    cmake -B build
)

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    echo Please ensure a C++ compiler (Visual Studio MSVC or MinGW GCC) is installed.
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

:: Step 4: Locate and Launch Executable Immediately
echo [4/4] Locating executable and starting engine...

if exist build\Release\Aengine3D.exe (
    pushd build\Release
    echo Running build\Release\Aengine3D.exe...
    Aengine3D.exe %*
    popd
) else if exist build\Debug\Aengine3D.exe (
    pushd build\Debug
    echo Running build\Debug\Aengine3D.exe...
    Aengine3D.exe %*
    popd
) else if exist build\Aengine3D.exe (
    pushd build
    echo Running build\Aengine3D.exe...
    Aengine3D.exe %*
    popd
) else (
    echo [ERROR] Executable Aengine3D.exe was not found in build directory.
    pause
    exit /b 1
)

endlocal
