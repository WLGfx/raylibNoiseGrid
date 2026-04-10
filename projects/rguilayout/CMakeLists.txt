# rguilayout CMakeLists.txt

# Add executable
add_executable(rguilayout 
    src/rguilayout.c
    src/external/tinyfiledialogs.c
)

# Include directories
target_include_directories(rguilayout PRIVATE 
    ${CMAKE_SOURCE_DIR}/projects/rguilayout
    ${CMAKE_SOURCE_DIR}/external/raylib/src
    ${CMAKE_SOURCE_DIR}/external/raygui/src
    ${CMAKE_SOURCE_DIR}/external
    )

# Link libraries
target_link_libraries(rguilayout PRIVATE raylib)
