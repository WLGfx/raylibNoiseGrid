#!/bin/bash

# Build all projects script
echo "Building all Raylib projects..."

# Create build directory
mkdir -p build
cd build

# Configure with new CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build all projects
cmake --build . --config Release

echo "Build complete! Executables are in build/bin/"
ls -la bin/
