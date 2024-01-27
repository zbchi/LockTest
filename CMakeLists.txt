cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-x c++ -std=c++11)

project(LockTest
        VERSION 2024.1
        DESCRIPTION "Xiyou Linux Group LockTest Task"
        LANGUAGE C CXX)

# Compilre setup
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb --fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")

# Output directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
set(LOCK_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

include_directories(${LOCK_SRC_INCLUDE_DIR})
include_directories(BEFORE src)