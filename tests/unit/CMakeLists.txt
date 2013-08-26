message ("Cmaking unit")

include_directories ("${GTEST_DIR}/include" "${GMOCK_DIR}/include" ../../src)

add_executable (test_main
    test_main.cc
    test_app.cc
    test_options.cc
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_GTEST_MAIN")

target_link_libraries (test_main app gtest gtest_main gmock pthread)

add_test (MainTest test_main)
