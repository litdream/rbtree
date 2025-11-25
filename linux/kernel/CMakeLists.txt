cmake_minimum_required(VERSION 3.14)
project(rbtree_test)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C standard for the rbtree implementation
set(CMAKE_C_STANDARD 99)

# GoogleTest requires at least C++11
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Create a library for the rbtree implementation
add_library(rbtree
  rbtree.c
  rbtree.h
  rbtree_types.h
  rbtree_augmented.h
)

# Set include directory for the library
target_include_directories(rbtree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Compiler flags for rbtree library
target_compile_options(rbtree PRIVATE -Wall -Wextra)

# Create the test executable
add_executable(rbtree_unittest
  rbtree_unittest.cpp
)

# Link the test executable with googletest and rbtree library
target_link_libraries(rbtree_unittest
  GTest::gtest_main
  rbtree
)

# Include GoogleTest module
include(GoogleTest)
gtest_discover_tests(rbtree_unittest)
