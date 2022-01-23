/*
 * Functions for handling parsing data from a file in various ways.
*/

#include <stdio.h>

#include "findex.h"

size_t scan_until_delimiter(FILE *findex, char delim, size_t length, char *buffer) {
    int byte = byte = fgetc(findex);
    size_t cursor = 0;

    while(1) {
        if(byte == EOF || byte == delim)
            break;

        if(cursor == length)
            break;

        buffer[cursor] = (char) byte;
        byte = fgetc(findex);
        cursor++;
    }

    buffer[cursor] = '\0';

    return cursor;
}

void ignore_until_delimiter(FILE *findex, char delim) {
    int byte = byte = fgetc(findex);

    while(1) {
        if(byte == EOF || byte == delim)
            break;

        byte = fgetc(findex);
    }
}

int ignore_until_delimiter_nl(FILE *findex, char delim) {
    int byte = fgetc(findex);

    while(1) {
        if(byte == '\n')
            return 1;

        if(byte == EOF || byte == delim)
            break;

        byte = fgetc(findex);
    }

    return 0;
}

void print_until_delimiter(FILE *findex, char delim) {
    int byte = fgetc(findex);

    while(1) {
        if(byte == EOF || byte == delim || byte == '\n')
            break;

        printf("%c", byte);

        byte = fgetc(findex);
    }
}

