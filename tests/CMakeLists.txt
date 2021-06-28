include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# If pkg-config is not installed on the system, then the
# CHECK_INSTALL_DIR variable must be set to the install
# location of Check. For example, on Windows, this may
# be: C:/Program Files/check
# set(CHECK_INSTALL_DIR "C:/Program Files/check")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

set(TEST_SOURCES
    test_tichy_strings.c
    test_tichy_sets.c
    test_tichy_translate.c

    test_vcdiff_cache.c
    test_vcdiff_window.c
    test_vcdiff_code_table.c
    test_vcdiff_file_allocation.c
    test_vcdiff_io.c
)


set(TEST_HEADERS
    test_tichy_strings.h
)

add_executable(test_tichy ${TEST_HEADERS} ${TEST_SOURCES})
target_link_libraries(test_tichy tichy_strings ${CHECK_LIBRARIES})
