message ("-- cmaking unit")

include_directories ("${GTEST_DIR}/include" "${GMOCK_DIR}/include" ../../src)

add_executable (test_main
    test_main.cc
    test_app.cc
    test_option.cc
    )
add_test (MainTest test_main)

add_definitions("${Debug_DEFINITIONS} -DUSE_GTEST_MAIN")

target_link_libraries (test_main app gtest gtest_main gmock pthread
    ${Boost_LIBRARIES}
    ${Debug_LIBRARIES})

