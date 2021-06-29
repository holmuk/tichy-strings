set(MAIN_SOURCES
    main.c
)

set(TICHY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(HEADERS
    ${TICHY_INCLUDE_DIR}/tichy_strings.h
    ${TICHY_INCLUDE_DIR}/vcdiff.h
)

add_executable(tichydiff ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(tichydiff tichy_strings)
