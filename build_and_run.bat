@echo off
setlocal enableextensions

echo ===================================================
echo   Aengine 3D Game Engine - Auto Build & Run Script
echo ===================================================

:: Step 1: Locate cmake.exe and compiler toolchain explicitly
echo [1/4] Environment initialization...

set "CMAKE_EXE="
set "CMAKE_GEN="

if exist "C:\msys64\ucrt64\bin\cmake.exe" (
    set "CMAKE_EXE=C:\msys64\ucrt64\bin\cmake.exe"
    set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    set "CMAKE_GEN=-G ""MinGW Makefiles"""
) else if exist "C:\msys64\mingw64\bin\cmake.exe" (
    set "CMAKE_EXE=C:\msys64\mingw64\bin\cmake.exe"
    set "PATH=C:\msys64\mingw64\bin;%PATH%"
    set "CMAKE_GEN=-G ""MinGW Makefiles"""
) else if exist "C:\msys64\clang64\bin\cmake.exe" (
    set "CMAKE_EXE=C:\msys64\clang64\bin\cmake.exe"
    set "PATH=C:\msys64\clang64\bin;%PATH%"
    set "CMAKE_GEN=-G ""MinGW Makefiles"""
) else if exist "C:\Program Files\CMake\bin\cmake.exe" (
    set "CMAKE_EXE=C:\Program Files\CMake\bin\cmake.exe"
    set "PATH=C:\Program Files\CMake\bin;%PATH%"
) else (
    for /f "delims=" %%I in ('where cmake.exe 2^>nul') do (
        if not defined CMAKE_EXE set "CMAKE_EXE=%%I"
    )
)

if not defined CMAKE_EXE (
    echo [ERROR] 'cmake.exe' was not found on this system!
    echo Please install CMake from https://cmake.org/download/ or MSYS2 from https://www.msys2.org/.
    pause
    exit /b 1
)

echo [OK] Found CMake at: "%CMAKE_EXE%"

:: Step 2: Configure project
echo [2/4] Configuring project via CMake...
if defined CMAKE_GEN (
    "%CMAKE_EXE%" -B build %CMAKE_GEN%
) else (
    "%CMAKE_EXE%" -B build
)

if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b %ERRORLEVEL%
)

:: Step 3: Build project
echo [3/4] Compiling Aengine 3D...
"%CMAKE_EXE%" --build build --config Release
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed! Check compiler output above.
    pause
    exit /b %ERRORLEVEL%
)

:: Step 4: Launch Executable
echo [4/4] Starting Aengine 3D Executable...

if exist "build\Release\Aengine3D.exe" (
    pushd "build\Release"
    Aengine3D.exe %*
    popd
) else if exist "build\Debug\Aengine3D.exe" (
    pushd "build\Debug"
    Aengine3D.exe %*
    popd
) else if exist "build\Aengine3D.exe" (
    pushd "build"
    Aengine3D.exe %*
    popd
) else (
    echo [ERROR] Executable Aengine3D.exe was not found in build directory.
    pause
    exit /b 1
)

endlocal
