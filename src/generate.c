#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#include "findex.h"
#include "libarg/libarg.h"

/* findex_gen
 *
 * Generate a findex database.
 */


/* The output file. */
FILE* outputFile;

/* The key name. */
char* attrName;

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
        printf("%s", "Usage: findex_gen FILE [--help | -h] [--file | -f filename] [--attr | -a attribute]\n");
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
    struct dirent* de;
    DIR* dr = opendir(".");
    char desc[384]; /* This is plenty of room, "trust me" */
    if(dr == NULL) {
        printf("Failed to open current directory.\n");
        return;
    }
    while((de = readdir(dr)) != NULL) {
	if(!((strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0))) {
            printf("%s for %s? ", attrName, de->d_name);
            //memset(desc, 0x00, 512);
            gets(desc);
	    strtok(desc, "\n");
	    if(strcmp(desc, "") != 0) {
	            fprintf(outputFile, "%s"  FINDEX_TOKEN_CHAR "%s"  FINDEX_PAIR_CHAR "%s\n", de->d_name, attrName, desc);
	    }
	}
    }
}


int main(int argc, char *argv[]) {
    char* outputFilename;

    struct LibargArgument arguments[1] = {
        {LIBARG_ARGUMENT_NULL}
    };
    struct LibargOption options[4] = {
        {"--help", "-h", NULL, NULL, 0},
        {"--file", "-f", NULL, NULL, 1},
	{"--attr", "-a", NULL, NULL, 1},
        {LIBARG_OPTION_NULL}
    };
    struct LibargProgram program = {"findex_gen", argv, argc, options, arguments};

    if(libarg_get_option("--file", program) != -1) {
        outputFilename = argv[libarg_get_option("--file", program) + 1];
    }
    else {
        outputFilename = ".findex";
    }
    
    if(libarg_get_option("--attr", program) != -1) {
	attrName = argv[libarg_get_option("--attr", program) + 1];
    }
    else {
	attrName = "description";
    }

    check_errors(program, outputFilename);

    printf("Generating database file...\nStrike RETURN to skip a line.\n");
    outputFile = fopen(outputFilename, "w");
    if(!outputFile) {
        printf("Failed to open %s\n", outputFilename);
        return -1;
    }

    populate_database();

    return EXIT_SUCCESS;
}
