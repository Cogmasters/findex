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

void check_errors(struct LibargProgram program, char *idxFile) {
    /* Display the help message */
    if(libarg_get_option("--help", program) != -1 || libarg_get_option("-h", program) != -1) {
        printf("%s", "Usage: findex FILE [KEY] [--help | -h]\n");
        printf("%s", "Retrieve file metatata from a file index table.\n");
        printf("%s", "\n");
        printf("%s", "    --help, -h      display this message\n");
        printf("%s", "    --file, -f      specify an alternate .findex file\n");

        exit(EXIT_SUCCESS);
    }

    libarg_error(program);

    /* File index table must exist in the directory findex is invoked in */
    if(file_exists(idxFile) == 0) {
        fprintf(stderr, "findex: could not find file index table\n");
        fprintf(stderr, "Try 'findex --help' for more information.\n");

        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    struct LibargArgument arguments[3] = {
        {"file", NULL, NULL, LIBARG_POSITIONAL},
        {"key", NULL, NULL, LIBARG_OPTIONAL},
        {LIBARG_ARGUMENT_NULL}
    };
    struct LibargOption options[3] = {
        {"--help", "-h", NULL, NULL, 0},
        {"--file", "-f", NULL, NULL, 1},
        {LIBARG_OPTION_NULL}
    };
    struct LibargProgram program;
    char* findex_filename;
    
    /* C89 workaround because immediate structs must be static */
    program.name = "findex";
    program.argv = argv;
    program.argc = argc;
    program.options = options;
    program.arguments = arguments;

    if(libarg_get_option("--file", program) != -1) {
        findex_filename = argv[libarg_get_option("--file", program) + 1];
    } else {
        findex_filename = ".findex";
    }

    check_errors(program, findex_filename);
    lookup_file(libarg_get_argument(program, "file"), libarg_get_argument(program, "key"), findex_filename);

    return EXIT_SUCCESS;
}
