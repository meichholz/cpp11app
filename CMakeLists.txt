project (cppeleven)

cmake_minimum_required (VERSION 2.6)
enable_testing ()
message("-- cmaking root")
# set/add compiler definitions FOR ALL TARGETS
add_definitions( "-Wall -Werror -std=c++0x -D_GLIBC_USE_NANOSLEEP" )
set (MAJOR_VERSION "0")
set (MINOR_VERSION "0")
set (PATCH_VERSION "1")
set (AUTHOR_NAME "Marian Eichholz")
set (AUTHOR_EMAIL "marian@bugslayer.de")
# setup boost libraries (/usr/share/cmake-2.8/Modules/FindBoost.cmake)
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package( Boost 1.49.0 COMPONENTS regex )
# boost consequences in local CMakeLists

set (DEBUG OFF CACHE BOOL "build instrumented code for gcov or profiling")
# some automated testing should do fine
set (GTEST_DIR "${CMAKE_SOURCE_DIR}/googletest/gtest-1.7.0" CACHE STRING "source base directory of google test framework")
set (GMOCK_DIR "${CMAKE_SOURCE_DIR}/googletest/gmock-1.7.0" CACHE STRING
	"source base directory of google mock framework")

if (DEBUG)
        message("-- building with tests and coverage support")
        set (Debug_DEFINITIONS "-fprofile-arcs -ftest-coverage")
        add_definitions( "-O0 -fPIC")
        set (Debug_LIBRARIES "gcov")
else (DEBUG)
        add_definitions( "-O0")
        set (Debug_LIBRARIES "")
endif (DEBUG)


# subdirectories and -projects
add_subdirectory (src)
if (DEBUG)
        add_subdirectory (gtest)
        add_subdirectory (tests)
endif (DEBUG)
configure_file(config.h.in config.h)
# testing: force output of (failing) test program
configure_file(CTestCustom.ctest.in CTestCustom.ctest)
# set debian package values
include (InstallRequiredSystemLibraries)
set (CPACK_SET_DESTDIR "on")
set (CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
set (CPACK_GENERATOR "DEB")
# set debian package stuff
set (CPACK_PACKAGE_DESCRIPTION "cppeleven - a cpp demo program")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ein einfaches Trainingsprogramm, und
eine Fingeruebung in Packaging.")
set (CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set (CPACK_DEBIAN_PACKAGE_SECTION "development")
# semi-fixed package settings
set (CPACK_PACKAGE_VENDOR "${AUTHOR_NAME}")
set (CPACK_PACKAGE_CONTACT "${AUTHOR_EMAIL}")
set (CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
set (CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
set (CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
set (CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
set (CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
#demo dependencies
set (CPACK_DEBIAN_PACKAGE_DEPENDS " libc6 (>= 2.1.3) ")

include (CPack)

