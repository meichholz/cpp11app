message ("Cmaking functional tests")

set (FUNCTIONAL_TEST_DIR ${CMAKE_SOURCE_DIR}/tests/functional)
add_test (FunctionalTestSuite ${FUNCTIONAL_TEST_DIR}/suite.sh)
