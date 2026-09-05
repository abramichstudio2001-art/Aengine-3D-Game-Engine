#!/usr/bin/env bash
set -e

echo "=== Building Aengine 3D Game Engine ==="
cmake -B build
cmake --build build --config Release

echo "=== Running Aengine 3D Game Engine ==="
if [ -f "./build/Aengine3D" ]; then
    ./build/Aengine3D "$@"
elif [ -f "./build/Release/Aengine3D" ]; then
    ./build/Release/Aengine3D "$@"
else
    echo "Executable not found in build directory."
    exit 1
fi
