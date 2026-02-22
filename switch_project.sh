#!/bin/bash

# Switch between projects script
PROJECT_NAME=$1

if [ -z "$PROJECT_NAME" ]; then
    echo "Usage: ./switch_project.sh [NoiseGrid|rguilayout|NewProject]"
    echo "Available projects:"
    ls -1 projects/
    exit 1
fi

if [ ! -d "projects/$PROJECT_NAME" ]; then
    echo "Project '$PROJECT_NAME' not found!"
    echo "Available projects:"
    ls -1 projects/
    exit 1
fi

echo "Switching to project: $PROJECT_NAME"

# Backup current CMakeLists.txt
if [ -f "CMakeLists.txt" ]; then
    cp CMakeLists.txt CMakeLists_backup.txt
fi

# Create project-specific CMakeLists.txt
cat > CMakeLists.txt << EOF
cmake_minimum_required(VERSION 3.15)
project($PROJECT_NAME)

# Compiler optimizations
add_compile_options(-O0 -march=native -flto -fno-exceptions)
add_compile_definitions(NDEBUG)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY \${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY \${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY \${CMAKE_BINARY_DIR}/bin)

# Add raylib
add_subdirectory(external/raylib)
include_directories(\${CMAKE_SOURCE_DIR}/external/raylib/src)
include_directories(\${CMAKE_SOURCE_DIR}/external/raylib/src/external/glfw/include)

# Add raygui headers
include_directories(\${CMAKE_SOURCE_DIR}/external/raygui/src)

# Add project
add_subdirectory(projects/$PROJECT_NAME)

# Enable link-time optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
EOF

echo "Switched to $PROJECT_NAME. Run 'cmake -B build && cmake --build build' to build."
