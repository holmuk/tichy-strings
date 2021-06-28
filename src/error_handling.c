#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "error_handling.h"


static void print_msg(const char *type, const char *msg, ...)
{
    va_list args;
    fprintf(stderr, "%s: ", type);
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
}


void print_error(const char *error, ...)
{
    va_list args;
    va_start(args, error);
    print_msg("ERROR", error, args);
    va_end(args);
}


void print_warning(const char *warning, ...)
{
    va_list args;
    va_start(args, warning);
    print_msg("WARNING", warning, args);
    va_end(args);
}
