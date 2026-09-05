#!/usr/bin/env bash
set -e

echo "=== Building Aengine 3D Game Engine ==="
cmake -B build
cmake --build build

echo "=== Running Aengine 3D Game Engine ==="
./build/Aengine3D "$@"
