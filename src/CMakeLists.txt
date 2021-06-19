set(LIB_SOURCES
    tichy_strings.c
)

set (TICHY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(HEADERS
    ${TICHY_INCLUDE_DIR}/tichy_strings.h
)

add_library(tichy_strings STATIC ${LIB_SOURCES} ${HEADERS})
