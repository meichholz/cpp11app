message ("Cmaking unit")
include_directories ("${GTEST_DIR}/include" "${GMOCK_DIR}/include" ../../src)
add_executable (test_main test_main.cc)
target_link_libraries (test_main app gtest gmock pthread)

add_test (MainTest test_main)
