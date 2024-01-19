cmake_minimum_required(VERSION 3.14)
project(Strbuf CXX C)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-x c++ -std=c++11)

set(SOURCE_FILES Lock/test.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -fsanitize=thread -g")
add_definitions(-D__SA_TEST__)

# Add source file
file(GLOB SRC Lock/*.c)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Locate and configure Google Test
 #find_package(GTest REQUIRED)
 #include_directories(${GTEST_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
  googletest

  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

# Set to OFF to prevent downloading libgmock
set(BUILD_GMOCK OFF CACHE BOOL "Builds the googlemock subproject" FORCE)

# Set to OFF to prevent downloading libgtest
set(BUILD_GTEST OFF CACHE BOOL "Builds the googletest subproject" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(Lock ${SRC} ${SOURCE_FILES} )

target_link_libraries(Lock gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(Lock)
