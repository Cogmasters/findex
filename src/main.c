#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "findex.h"
#include "libarg/libarg.h"

int file_exists(const char *path) {
    struct stat stat_buffer = {0};

    if(stat(path, &stat_buffer) == -1)
        return 0;

    return 1;
}

void check_errors(struct LibargProgram program) {
    /* Display the help message */
    if(libarg_get_option("--help", program) != -1 || libarg_get_option("-h", program) != -1) {
        printf("%s", "Usage: findex FILE [KEY] [--help | -h]\n");
        printf("%s", "Retrieve file metatata from a file index table.\n");
        printf("%s", "\n");
        printf("%s", "    --help, -h      display this message\n");

        exit(EXIT_SUCCESS);
    }

    libarg_error(program);

    /* File index table must exist in the directory findex is invoked in */
    if(file_exists("./.findex") == 0) {
        fprintf(stderr, "findex: could not find file index table\n");
        fprintf(stderr, "Try 'findex --help' for more information.\n");

        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    struct LibargArgument arguments[3] = {
        {"file", NULL, NULL, LIBARG_POSITIONAL},
        {"key", NULL, NULL, LIBARG_POSITIONAL},
        {LIBARG_ARGUMENT_NULL}
    };
    struct LibargOption options[2] = {
        {"--help", "-h", NULL, NULL, 0},
        {LIBARG_OPTION_NULL}
    };
    struct LibargProgram program = {"findex", argv, argc, options, arguments};

    check_errors(program);
    lookup_file(libarg_get_argument(program, "file"), libarg_get_argument(program, "key"));

    return EXIT_SUCCESS;
}
