cmake_minimum_required(VERSION 3.10)
project(NES_Emulator)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SRC_FILES src/*.c)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(NES_Emulator main.c ${SRC_FILES})