message ("-- cmaking src")
# add library in place

add_library(app app.cc globals.cc option.cc showcase.cc)
add_executable(cppeleven main.cc)
# if(Boost_FOUND)
# endif()
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(${Debug_DEFINITIONS})
target_link_libraries(cppeleven app ${Boost_LIBRARIES} ${Debug_LIBRARIES})

install (TARGETS cppeleven DESTINATION bin)
