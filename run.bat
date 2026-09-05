@echo off
echo === Building Aengine 3D Game Engine ===
cmake -B build
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

rem Prepend build output folders and standard MSYS2 / MinGW bin paths to PATH
set PATH=%CD%\build;%CD%\build\Release;%CD%\build\Debug;C:\msys64\mingw64\bin;C:\MinGW\bin;%PATH%

rem Copy any DLLs from build root or common MinGW/MSYS2 locations if available
for %%D in (C:\msys64\mingw64\bin C:\MinGW\bin) do (
    if exist "%%D\glew32.dll" (
        if exist build\Release xcopy /y /d "%%D\glew32.dll" build\Release\ >nul 2>&1
        if exist build\Debug xcopy /y /d "%%D\glew32.dll" build\Debug\ >nul 2>&1
        if exist build xcopy /y /d "%%D\glew32.dll" build\ >nul 2>&1
    )
    if exist "%%D\glfw3.dll" (
        if exist build\Release xcopy /y /d "%%D\glfw3.dll" build\Release\ >nul 2>&1
        if exist build\Debug xcopy /y /d "%%D\glfw3.dll" build\Debug\ >nul 2>&1
        if exist build xcopy /y /d "%%D\glfw3.dll" build\ >nul 2>&1
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
