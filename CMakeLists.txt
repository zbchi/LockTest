cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(LockTest
        VERSION 2024.1
        DESCRIPTION "Xiyou Linux Group LockTest Task"
        LANGUAGES C CXX)

# Compilre setup
if(NOT DEFINED LOCK_SANITIZER)
        set(LOCK_SANITIZER thread)
endif()

message("${LOCK_SANITIZER} sanitizer will be enabled.")

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${LOCK_SANITIZER}")

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

# Output directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
set(LOCK_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
# set(LOCK_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/test.h)

include_directories(${LOCK_SRC_INCLUDE_DIR})
include_directories(BEFORE src)

# Other CMake modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

add_subdirectory(src)
add_subdirectory(test)