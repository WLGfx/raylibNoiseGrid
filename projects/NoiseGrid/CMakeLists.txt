# NoiseGrid Project CMakeLists.txt

# Add executable
add_executable(NoiseGrid 
    src/main.cpp
    inc/FastNoise.cpp
    inc/NoiseGrid.cpp
    inc/NoiseChunk.cpp
    inc/raygui+.cpp
)

# Include directories
target_include_directories(NoiseGrid PRIVATE 
    ${CMAKE_SOURCE_DIR}/projects/NoiseGrid
    ${CMAKE_SOURCE_DIR}/external/raylib/src
    ${CMAKE_SOURCE_DIR}/external/raygui/src
)

# Link libraries
target_link_libraries(NoiseGrid PRIVATE raylib)
