# NewProject CMakeLists.txt

# Add executable
add_executable(rlNoise2 
    src/main.cpp
    inc/rui.cpp
    inc/Game.cpp
)

# Include directories
target_include_directories(rlNoise2 PRIVATE 
    ${CMAKE_SOURCE_DIR}/projects/rlNoise2
    ${CMAKE_SOURCE_DIR}/external/raylib/src
    ${CMAKE_SOURCE_DIR}/external/raygui/src
)

# Link libraries
target_link_libraries(rlNoise2 PRIVATE raylib)
