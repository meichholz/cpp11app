message ("Cmaking src")
# add library in place
add_library(app app.cc showcase.cc)
add_executable(cppeleven main.cc)
target_link_libraries(cppeleven app)
install (TARGETS cppeleven DESTINATION bin)


