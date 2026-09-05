@echo off
echo === Building Aengine 3D Game Engine ===
cmake -B build
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

echo === Running Aengine 3D Game Engine ===
if exist build\Release\Aengine3D.exe (
    build\Release\Aengine3D.exe %*
) else if exist build\Aengine3D.exe (
    build\Aengine3D.exe %*
) else (
    echo Executable not found in build directory.
    exit /b 1
)
