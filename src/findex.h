#ifndef FINDEX_H
#define FINDEX_H

#define FINDEX_MAX_FILENAME 512
#define FINDEX_MAX_KEYNAME  512

/* Delimiters */
#define FINDEX_TOKEN_DELIM  0x01
#define FINDEX_PAIR_DELIM   0x05

#define FINDEX_TOKEN_CHAR  "\x01"
#define FINDEX_PAIR_CHAR   "\x05"
/*
 * Searches the index table in the directory of the process
 * for a file, and extracts the key provided. Data is printed
 * to the stdout.
 *
 * @param file: the file to search for
 * @param key: the key to extract
*/
void lookup_file(const char *file, const char *key);

/*
 * Scan a file until a delimiter is met, and write the bytes
 * scanned to a buffer of length LENGTH. The resulting buffer
 * will be NUL terminated.
 *
 * @param findex: the index table to search
 * @param delim: the delimiter to scan until
 * @param length: the maximum length of the buffr
 * @param buffer: the buffer to write to
 * @return: number of bytes written
*/
size_t scan_until_delimiter(FILE *findex, char delim, size_t length, char *buffer);

/*
 * Scan a file until a delimiter is met, and do nothing with
 * them. This is used to, quite literally, just get data in
 * the file out of the way.
 *
 * @param findex: the index table to search
 * @param delim: the delimiter to scan until
*/
void ignore_until_delimiter(FILE *findex, char delim);

/*
 * Scan a file until a delimiter is met, and do nothing with
 * them. This is used to, quite literally, just get data in
 * the file out of the way. If, however, a new line is found
 * somewhere in the stream before the delimiter, then the
 * function returns.
 *
 * @param findex: the index table to search
 * @param delim: the delimiter to scan until
 * @return: 1 if a newline was found, 0 if it was not
*/
int ignore_until_delimiter_nl(FILE *findex, char delim);

/*
 * Print the contents of a file up until a delimiter is met.
 *
 * @param findex: the index table to search
 * @param delim: the delimiter to scan until
*/
void print_until_delimiter(FILE *findex, char delim);

/*
 * Print the contents of a file up until a delimiter is met,
 * but return from the function when, or if a new line is
 * found.
 *
 * @param findex: the index table to search
 * @param delim: the delimiter to scan until
 * @return: whether or not a new line was found
*/
int print_until_delimiter_nl(FILE *findex, char delim);

#endif
