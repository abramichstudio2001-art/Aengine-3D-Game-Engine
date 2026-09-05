#!/usr/bin/env bash
set -e

echo "==================================================="
echo "  Aengine 3D Game Engine - Auto Build & Run Script"
echo "==================================================="

# Step 1: Check prerequisites
echo "[1/4] Checking prerequisites..."
if ! command -v cmake &> /dev/null; then
    echo "[ERROR] 'cmake' was not found in PATH."
    echo "Please install CMake (e.g. 'sudo apt install cmake' or 'brew install cmake') and try again."
    exit 1
fi
echo "[OK] CMake is available."

# Step 2: Configure project & download dependencies
echo "[2/4] Configuring project and downloading dependencies (GLFW, GLM, ImGui)..."
cmake -B build

# Step 3: Compile project
echo "[3/4] Compiling Aengine 3D..."
cmake --build build --config Release

# Step 4: Locate and Launch Executable
echo "[4/4] Locating executable and starting engine..."
if [ -f "./build/Aengine3D" ]; then
    ./build/Aengine3D "$@"
elif [ -f "./build/Release/Aengine3D" ]; then
    ./build/Release/Aengine3D "$@"
elif [ -f "./build/Debug/Aengine3D" ]; then
    ./build/Debug/Aengine3D "$@"
else
    echo "[ERROR] Executable Aengine3D not found in build directory."
    exit 1
fi
