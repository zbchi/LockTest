file(GLOB LOCK_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lock/*.c)

# add_library(lock STATIC ${LOCK_SRC})
add_library(lock_object OBJECT ${LOCK_SRC})
message(STATUS ${LOCK_SRC})
add_library(lock STATIC $<TARGET_OBJECTS:lock_object>)

find_package(Threads REQUIRED)

target_include_directories(
    lock PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include)