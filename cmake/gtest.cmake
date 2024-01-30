include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Set to OFF to prevent downloading libgmock
set(BUILD_GMOCK OFF CACHE BOOL "Builds the googlemock subproject" FORCE)

# Set to OFF to prevent downloading libgtest
set(BUILD_GTEST OFF CACHE BOOL "Builds the googletest subproject" FORCE)

FetchContent_MakeAvailable(googletest)