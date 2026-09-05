@echo off
setlocal enabledelayedexpansion

echo ===================================================
echo   Aengine 3D Game Engine - Auto Build & Run Script
echo ===================================================

:: Step 1: Detect CMake and fix DLL environment PATH
echo [1/4] Checking CMake and environment runtime PATH...

set "CMAKE_CMD="

:: Check if cmake is already in PATH
where cmake >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    for /f "delims=" %%I in ('where cmake') do (
        if not defined CMAKE_CMD (
            set "CMAKE_EXE=%%I"
            set "CMAKE_DIR=%%~dpI"
        )
    )
)

:: Auto-detect common MSYS2 / CMake locations if not found or to prioritize MSYS2 DLL environment
if exist "C:\msys64\ucrt64\bin\cmake.exe" (
    set "MSYS_BIN=C:\msys64\ucrt64\bin"
    set "PATH=C:\msys64\ucrt64\bin;!PATH!"
) else if exist "C:\msys64\mingw64\bin\cmake.exe" (
    set "MSYS_BIN=C:\msys64\mingw64\bin"
    set "PATH=C:\msys64\mingw64\bin;!PATH!"
) else if exist "C:\msys64\clang64\bin\cmake.exe" (
    set "MSYS_BIN=C:\msys64\clang64\bin"
    set "PATH=C:\msys64\clang64\bin;!PATH!"
) else if exist "C:\Program Files\CMake\bin\cmake.exe" (
    set "PATH=C:\Program Files\CMake\bin;!PATH!"
) else if defined CMAKE_DIR (
    set "PATH=!CMAKE_DIR!;!PATH!"
)

where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] 'cmake' was not found in PATH or standard MSYS2/CMake directories!
    echo Please install CMake or MSYS2 (https://www.msys2.org/) and add it to PATH.
    pause
    exit /b 1
)

echo [OK] CMake environment initialized.

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
