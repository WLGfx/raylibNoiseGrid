cmake_minimum_required(VERSION 3.15)
project(rlNoiseGrid)

#add_compile_options(-O3)  # Maximum optimization
#add_compile_options(-march=native)  # Optimize for the current CPU
#add_compile_options(-flto)  # Link-time optimization
#add_compile_options(-fno-exceptions)  # Disable exception handling if not used
#add_compile_definitions(NDEBUG)  # Disable debug code

#add_compile_options(-ffunction-sections -fdata-sections)
#add_link_options(-Wl,--gc-sections)

add_compile_options(-O0 -march=native -flto -fno-exceptions)
add_compile_definitions(NDEBUG)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add raylib
add_subdirectory(external/raylib)
include_directories(${CMAKE_SOURCE_DIR}/external/raylib/src)
include_directories(${CMAKE_SOURCE_DIR}/external/raylib/src/external/glfw/include)

# Add raygui
#add_subdirectory(external/raygui)
include_directories(${CMAKE_SOURCE_DIR}/external/raygui/src)

# Add executable
add_executable(${PROJECT_NAME} 
src/main.cpp
src/inc/FastNoise.h
src/inc/FastNoise.cpp
src/inc/NoiseGrid.h
src/inc/NoiseGrid.cpp
src/inc/NoiseChunk.cpp
)

# Add this to enable link-time optimization
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)