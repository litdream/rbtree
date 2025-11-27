# tests/CMakeLists.txt

# Add the test executable.
add_executable(rbtree_test test_rbtree.cpp test_rbtree_delete.cpp)

# Link the test executable against our rbtree library and GoogleTest libraries.
target_link_libraries(rbtree_test PRIVATE rbtree GTest::gtest_main)

# Make the rbtree headers visible to the test files.
target_include_directories(rbtree_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Include GoogleTest's discover tests functionality.
include(GoogleTest)
gtest_discover_tests(rbtree_test)
