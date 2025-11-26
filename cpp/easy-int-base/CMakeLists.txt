# CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(RBTreeProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library Definition ---
# Create a library from our rbtree source files.
add_library(rbtree rbtree.cpp rbtree.hpp)

# Make the header files available to other targets (like our tests).
target_include_directories(rbtree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(rbtree PUBLIC UNIT_TEST_BUILD)

# Link against gtest to make headers available for FRIEND_TEST
target_link_libraries(rbtree PUBLIC GTest::gtest)

# --- GoogleTest ---
# Download and configure GoogleTest.
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

FetchContent_MakeAvailable(googletest)

# --- Testing ---
# Enable testing with CTest.
enable_testing()

# Add the 'tests' subdirectory to the build.
add_subdirectory(tests)
