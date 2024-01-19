cmake_minimum_required(VERSION 3.14)
project(Lock CXX C)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-x c++ -std=c++11)

# Output directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
set(LOCK_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

include_directories(${LOCK_SRC_INCLUDE_DIR})
include_directories(BEFORE src)