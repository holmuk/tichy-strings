set(LIB_SOURCES
    error_handling.c

    tichy_minimal_covering.c
    tichy_strings.c

    vcdiff.c

    vcdiff_cache.c
    vcdiff_code_table.c
    vcdiff_encoder.c
    vcdiff_window.c
    vcdiff_io.c
)

set (TICHY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(HEADERS
    error_handling.h

    vcdiff_cache.h
    vcdiff_code_table.h
    vcdiff_encoder.h
    vcdiff_window.h
    vcdiff_io.h

    ${TICHY_INCLUDE_DIR}/tichy_strings.h
    ${TICHY_INCLUDE_DIR}/vcdiff.h
)

add_library(tichy_strings STATIC ${LIB_SOURCES} ${HEADERS})
