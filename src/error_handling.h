#ifndef VCDIFF_ERROR_HANDLING
#define VCDIFF_ERROR_HANDLING

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define FOPEN_ERROR_MSG "Failed to execute fopen()"
#define FREAD_ERROR_MSG "Failed to execute fread()"
#define FWRITE_ERROR_MSG "Failed to execute fwrite()"
#define FILESIZE_ERROR_MSG "Failed to determine the file size"


enum vcdiff_error_codes {
    // Global errors
    VCDIFF_OK,
    VCDIFF_MEMORY_ERROR,
    VCDIFF_GLOBAL_ERRORS_COUNTER,

    // IO
    VCDIFF_IO_INVALID_SOURCE_FILE,
    VCDIFF_IO_INVALID_INPUT_FILE,
    VCDIFF_IO_INVALID_OUTPUT_FILE,

    VCDIFF_IO_FOPEN_ERROR,
    VCDIFF_IO_FREAD_ERROR,
    VCDIFF_IO_FWRITE_ERROR,
    VCDIFF_IO_FILE_SIZE_ERROR,

    // General
    VCDIFF_NOT_YET_IMPLEMENTED,

    VCDIFF_ERRORS_NUMBER
};


/**
 * Print error message to stderr.
 * @param error Error message
 */
void print_error(const char *error, ...);


/**
 * Print warning message to stderr.
 * @param error Warning message
 */
void print_warning(const char *warning, ...);

#endif // VCDIFF_ERROR_HANDLING
