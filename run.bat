@echo off
rem Prepend common MSYS2 / MinGW / vcpkg binary directories and local build folders to PATH
set PATH=C:\msys64\mingw64\bin;C:\msys64\ucrt64\bin;C:\msys64\clang64\bin;C:\MinGW\bin;%CD%\build;%CD%\build\Release;%CD%\build\Debug;%PATH%

echo === Building Aengine 3D Game Engine ===
cmake -B build
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

rem Automatically copy dynamic libraries from toolchain bin directories to output folder
for %%D in (C:\msys64\mingw64\bin C:\msys64\ucrt64\bin C:\msys64\clang64\bin C:\MinGW\bin) do (
    if exist "%%D" (
        if exist build\Release xcopy /y /d "%%D\*.dll" build\Release\ >nul 2>&1
        if exist build\Debug xcopy /y /d "%%D\*.dll" build\Debug\ >nul 2>&1
        if exist build xcopy /y /d "%%D\*.dll" build\ >nul 2>&1
    )
)

if exist build\*.dll (
    if exist build\Release xcopy /y /d build\*.dll build\Release\ >nul 2>&1
    if exist build\Debug xcopy /y /d build\*.dll build\Debug\ >nul 2>&1
)

echo === Running Aengine 3D Game Engine ===
if exist build\Release\Aengine3D.exe (
    pushd build\Release
    Aengine3D.exe %*
    popd
) else if exist build\Debug\Aengine3D.exe (
    pushd build\Debug
    Aengine3D.exe %*
    popd
) else if exist build\Aengine3D.exe (
    pushd build
    Aengine3D.exe %*
    popd
) else (
    echo [ERROR] Executable not found in build directory.
    echo Please ensure CMake and a C++ compiler (MSVC / MinGW) are installed.
    echo If missing DLLs (e.g. glew32.dll or glfw3.dll), copy them into the build executable directory or add them to PATH.
    exit /b 1
)
