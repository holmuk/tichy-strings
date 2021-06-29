#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "tichy_strings.h"


/**
 * Parse SI units for bytes.
 * K (for kibibytes) and M (for mebibytes) prefixes are supported.
 * @param number Prefixed or non-prefixed number.
 * @return Number of bytes.
 */
static size_t parse_si_bytes(const char *number)
{
    int length = strlen(number);
    char prefix = number[length - 1];

    size_t multiplier = 1;
    if (prefix == 'K' || prefix == 'k') {
        multiplier = 1024;
    } else if (prefix == 'M' || prefix == 'm') {
        multiplier = 1024 * 1024;
    }

    size_t value = strtoll(number, NULL, 10) * multiplier;
    return value;
}


int main(int argc, char *argv[])
{
    int opt;

    static struct option long_options[] = {
        {"source", required_argument, 0, 's'},
        {"template", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"source-buffer-size", required_argument, 0, 'b'},
        {"template-buffer-size", required_argument, 0, 't'},
        {"output-buffer-size", required_argument, 0, 'z'},
        {NULL, 0, NULL, 0}
    };

    char *input_file = strdup("");
    char *output_file = strdup("");
    char *source_file = strdup("");

    int option_index = 0;
    size_t source_buffer_size = 0;
    size_t template_buffer_size = 0;
    size_t output_buffer_size = 0;

    while ((opt = getopt_long(argc, argv, "s:i:o:b:t:z:",
                              long_options, &option_index)) != -1)
    {
        switch (opt) {
            case 'i':
                free(input_file);
                input_file = strdup(optarg);
                break;

            case 'o':
                free(output_file);
                output_file = strdup(optarg);
                break;

            case 's':
                free(source_file);
                source_file = strdup(optarg);
                break;

            case 'b':
                source_buffer_size = parse_si_bytes(optarg);
                break;

            case 't':
                template_buffer_size = parse_si_bytes(optarg);
                break;

            case 'z':
                output_buffer_size = parse_si_bytes(optarg);
                break;

            default:
                fprintf(stderr,
                        "Usage: %s [-s source] [-i template] \n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    tichy_encode_vcdiff(
                source_file,
                input_file,
                output_file,
                source_buffer_size,
                template_buffer_size,
                output_buffer_size);

    free(input_file);
    free(output_file);
    free(source_file);

    return 0;
}
