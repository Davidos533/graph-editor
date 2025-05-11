cmake_minimum_required(VERSION 3.10)

# Project name
project(graph-editor)

# Set C++ standard
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.hpp" "include/*.h")

# Include directories
include_directories(include)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

# Add GLAD files (if not already in source directory)
# GLAD typically comes as a set of .c and .h files that you need to compile and link
# Assuming GLAD is in a subdirectory under src or include
add_library(glad "src/glad.c")
target_include_directories(glad PRIVATE "include")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    glad 
    ${OPENGL_LIBRARIES} 
    glfw 
    Freetype::Freetype
)
