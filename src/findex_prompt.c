#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "findex.h"
#include "libarg/libarg.h"

/* findex_prompt
 *
 * Prompt the user for values to insert into a .findex file.
 * Similar to findex_gen, but it does not require UNIX headers.
 */


/* The output file. */
FILE* output_file;

/* The key name. */
char* attr_name;

/* See if a file exists */
int file_exists(const char *path) {
    struct stat stat_buffer = {0};

    if(stat(path, &stat_buffer) == -1)
        return 0;

    return 1;
}

void check_errors(struct LibargProgram program, char* filename) {
    /* Display the help message, and tell the user to RTFM */
    if(libarg_get_option("--help", program) != -1 || libarg_get_option("-h", program) != -1) {
        printf("%s", "Usage: findex_prompt FILE [--help | -h] [--file | -f filename] [--attr | -a attribute]\n");
        printf("%s", "Produce a lookup database for findex.\n");
        printf("%s", "\n");
        printf("%s", "    --help, -h      display this message\n");
        printf("%s", "    --file, -f      change output file\n");
        printf("%s", "    --attr, -a      specify an attribute\n");

        exit(EXIT_SUCCESS);
    }

    libarg_error(program);

    /* Make sure that the user isn't about to blow away his or her database! */
    if(file_exists(filename) == 1) {
        fprintf(stderr, "Specified file already exists.\n");
        exit(EXIT_FAILURE);
    }
}

void populate_database() {
    char desc[FINDEX_DESC_LENGTH + 1]; /* This is plenty of room, "trust me" */
    char file[FINDEX_DESC_LENGTH + 1];
    for(;;) {
        printf("Filename? ");
        fgets(file, FINDEX_DESC_LENGTH, stdin);

        printf("Description? ", file);
        fgets(desc, FINDEX_DESC_LENGTH, stdin);

        if((strcmp(file, "\n") != 0) || (strcmp(desc, "\n") != 0)) {
            strtok(desc, "\n");
            strtok(file, "\n");
            fprintf(output_file, "%s"  FINDEX_TOKEN_CHAR "%s"  FINDEX_PAIR_CHAR "%s\n", file, attr_name, desc);
        }
        else {
            break;
        }
    }
}


int main(int argc, char *argv[]) {
    char* output_filename;

    struct LibargArgument arguments[1] = {
        {LIBARG_ARGUMENT_NULL}
    };
    struct LibargOption options[4] = {
        {"--help", "-h", NULL, NULL, 0},
        {"--file", "-f", NULL, NULL, 1},
        {"--attr", "-a", NULL, NULL, 1},
        {LIBARG_OPTION_NULL}
    };
    struct LibargProgram program;

    program.name = "findex_prompt";
    program.argv = argv;
    program.argc = argc;
    program.options = options;
    program.arguments = arguments;

    if(libarg_get_option("--file", program) != -1) {
        output_filename = argv[libarg_get_option("--file", program) + 1];
    }
    else {
        output_filename = ".findex";
    }

    if(libarg_get_option("--attr", program) != -1) {
	    attr_name = argv[libarg_get_option("--attr", program) + 1];
    }
    else {
	    attr_name = "description";
    }

    check_errors(program, output_filename);

    printf("Generating database file...\nRespond to prompts, enter a blank line to quit.\n");
    output_file = fopen(output_filename, "w");

    if(!output_file) {
        printf("Failed to open %s\n", output_filename);
        return -1;
    }

    populate_database();

    return EXIT_SUCCESS;
}
