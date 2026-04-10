# NoiseGrid Project CMakeLists.txt

# Add executable
add_executable(
    NoiseGrid 
    
    src/main.cpp
    inc/FastNoise/FastNoise.cpp
    inc/NoiseGrid/NoiseGrid.cpp
    inc/NoiseGrid/NoiseChunk.cpp
    inc/r/raygui+/raygui+.cpp
    inc/r/raygui+/rlgui.cpp
    inc/myui.cpp
    inc/ui2.cpp
    inc/r/rlshaders/rlshaders.cpp
    inc/r/rCamera/rCamera.cpp
    inc/r/implementations.cpp
)

# Include directories
target_include_directories(NoiseGrid PRIVATE 
    ${CMAKE_SOURCE_DIR}/projects/NoiseGrid
    ${CMAKE_SOURCE_DIR}/external/raylib/src
    ${CMAKE_SOURCE_DIR}/external/raygui/src
)

# Link libraries
target_link_libraries(NoiseGrid PRIVATE raylib)
