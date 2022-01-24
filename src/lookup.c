/*
 * Handles lookup from a file.
*/

#include <stdio.h>
#include <string.h>

#include "findex.h"

void dump_all_keys(FILE *findex, const char *name) {
    char key_name[FINDEX_MAX_KEYNAME + 1] = {0};

    /* Dump all keys until the value contains a new line, which signals
     * that this table entry is exhausted. */
    while(1) {
        scan_until_delimiter(findex, FINDEX_PAIR_DELIM, FINDEX_MAX_KEYNAME, key_name);

        printf("%s - %s:\n", name, key_name);

        if(print_until_delimiter_nl(findex, FINDEX_TOKEN_DELIM) == 1)
            break;

        printf("%c", '\n');
    }        

    printf("%s\n", key_name);
}

void extract_key(FILE *findex, const char *name, const char *key) {
    char key_name[FINDEX_MAX_KEYNAME + 1] = {0};

    /* Scan the keys until the correct key is found */
    while(strcmp(key_name, key) != 0) {
        if(scan_until_delimiter(findex, FINDEX_PAIR_DELIM, FINDEX_MAX_KEYNAME, key_name) == 0)
            break;

        /* Skip to the next key */
        if(strcmp(key_name, key) != 0) {

            /* If a newline is found while scanning the line, then that means
             * that this is the last key/value pair in the key, and thus the
             * key in question could not be found. */
            if(ignore_until_delimiter_nl(findex, FINDEX_TOKEN_DELIM) == 1)
                break;
        }
    }

    /* The key could not be found */
    if(strcmp(key_name, key) != 0) {
        return;
    }

    printf("%s - %s:\n", name, key);
    print_until_delimiter(findex, FINDEX_TOKEN_DELIM);
    printf("%c", '\n');
}

void lookup_file(const char *file, const char *key) {
    FILE *findex = fopen("./.findex", "r");
    char name[FINDEX_MAX_FILENAME + 1] = {0};

    /* Scan the file until the file with the correct name is found. */
    while(strcmp(name, file) != 0) {
        if(scan_until_delimiter(findex, FINDEX_TOKEN_DELIM, FINDEX_MAX_FILENAME, name) == 0)
            break;

        /* Skip to the next line */
        if(strcmp(name, file) != 0) {
            ignore_until_delimiter(findex, '\n');
        }
    }

    if(strcmp(name, file) != 0) {
        fclose(findex);

        return;
    }

    if(key == NULL) {
        dump_all_keys(findex, name);
    } else {
        extract_key(findex, name, key);
    }

    fclose(findex);
}
