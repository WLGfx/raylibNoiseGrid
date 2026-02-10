# NewProject CMakeLists.txt

# Add executable
add_executable(NewProject 
    src/main.cpp
    inc/rui.cpp
)

# Include directories
target_include_directories(NewProject PRIVATE 
    ${CMAKE_SOURCE_DIR}/projects/NewProject
    ${CMAKE_SOURCE_DIR}/external/raylib/src
    ${CMAKE_SOURCE_DIR}/external/raygui/src
)

# Link libraries
target_link_libraries(NewProject PRIVATE raylib)
