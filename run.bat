@echo off
echo === Building Aengine 3D Game Engine ===
cmake -B build
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 exit /b %ERRORLEVEL%

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
    exit /b 1
)
