cmake_minimum_required(VERSION 3.5)

project(tichy-strings-correction
    DESCRIPTION [=[
    Implementation of Walter F. Tichy's strings correction algorithm
    ]=]
    LANGUAGES C)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(tichydiff)

# Check unit-tests
enable_testing()
add_test(NAME test_tichy COMMAND test_tichy)
