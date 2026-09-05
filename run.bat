@echo off
setlocal

echo ===================================================
echo   Aengine 3D Game Engine - Auto Build & Run Script
echo ===================================================

:: Step 1: Prioritize MSYS2 / MinGW runtime DLL path to prevent exit code 0xC0000139
echo [1/4] Environment initialization and DLL path search setup...

set "CMAKE_GEN="

if exist "C:\msys64\ucrt64\bin\cmake.exe" (
    set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    set CMAKE_GEN=-G "MinGW Makefiles"
) else if exist "C:\msys64\mingw64\bin\cmake.exe" (
    set "PATH=C:\msys64\mingw64\bin;%PATH%"
    set CMAKE_GEN=-G "MinGW Makefiles"
) else if exist "C:\msys64\clang64\bin\cmake.exe" (
    set "PATH=C:\msys64\clang64\bin;%PATH%"
    set CMAKE_GEN=-G "MinGW Makefiles"
) else if exist "C:\Program Files\CMake\bin\cmake.exe" (
    set "PATH=C:\Program Files\CMake\bin;%PATH%"
)

where gcc >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    if "%CMAKE_GEN%"=="" set CMAKE_GEN=-G "MinGW Makefiles"
)

where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] 'cmake' was not found in PATH or standard MSYS2/CMake installation paths!
    echo Please install CMake (https://cmake.org/download/) or MSYS2 (https://www.msys2.org/).
    pause
    exit /b 1
)

echo [OK] Environment initialized with matching runtime DLL search path.

:: Step 2: Configure project and download dependencies via FetchContent
echo [2/4] Configuring project and downloading dependencies (GLFW, GLM, ImGui)...
if not "%CMAKE_GEN%"=="" (
    cmake -B build %CMAKE_GEN%
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
