cmake_minimum_required(VERSION 3.10)
project(MyCProject)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Gather all source files in the src directory
file(GLOB_RECURSE SRC_FILES src/*.c)

# Add the executable
add_executable(my_program ${SRC_FILES})