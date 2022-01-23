#ifndef LIBARG_H
#define LIBARG_H

/* Argument types */
#define LIBARG_POSITIONAL   0
#define LIBARG_OPTIONAL     1
#define LIBARG_VARIADIC     2

/* Iterator macros */
#define libarg_option_iter(option, program, index)                                                   \
    (index) = libarg_get_option((option), (program)) + 1;                                            \
    (index) < libarg_get_option((option), (program)) + libarg_count_parameters((option), (program)); \
    (index)++

#define libarg_variadic_iter(program, index) \
    (index) = libarg_find_variadic((program)); (index) != -1; (index) = libarg_next_variadic((program), (index))

/* NULL terminators */
#define LIBARG_OPTION_NULL      NULL, NULL, NULL, NULL, 0
#define LIBARG_ARGUMENT_NULL    NULL, NULL, NULL, 0

/*
 * Represents a positional argument that the program can accept.
 * No positional or optional arguments can be accepted after a
 * variadic argument.
*/
struct LibargArgument {
    char *name;
    char *description;
    char *fallback;
    int type;
};

/*
 * Represents an option that the program can accept. Options are
 * inherently optional. If the number of parameters is 0, then
 * it is treated as a flag. While flags are not separate, it is
 * worth noting it.
*/
struct LibargOption {
    char *longform;
    char *shortform;
    char *description;
    char *fallback;
    int parameters;
};

/*
 * Metadata about the program.
*/
struct LibargProgram {
    char *name;
    char **argv;
    int argc;
    struct LibargOption *options;
    struct LibargArgument *arguments;
};

/*
 * Counts the number of positional arguments registered
 * into the program.
 *
 * @param arguments: the argument array to count
 * @return: the number of positional arguments
*/
int libarg_count_arguments(struct LibargArgument *arguments);

/*
 * Counts the number of optional arguments registered
 * into the program.
 *
 * @param options: the option array to count
 * @return: the number of options
*/
int libarg_count_options(struct LibargOption *options);

/*
 * Finds the location of an option in an option array.
 * Checks both long and short form arguments.
 *
 * @param name: name: the name of the option
 * @param options: the option array to search
 * @return: the index of the option, or -1 if it does not exist
*/
int libarg_find_option(const char *name, struct LibargOption *options);

/*
 * Retrieves the index of an option in a program's argv.
 * The option does not need to be registered in the program's
 * arguments for this to search for it.
 *
 * @param name: the name of the option
 * @param program: the program to search
 * @return: the index of the option or -1 if it does not exist
*/
int libarg_get_option(const char *name, struct LibargProgram program);

/*
 * Counts the number of parameters that an option has inside
 * the argv, as opposed to how many parameters it is intended
 * to have. Works for variadic options.
 *
 * @param name: the name of the option
 * @param program: the program container to search
 * @return: the number of parameters given to the option
*/
int libarg_count_parameters(const char *name, struct LibargProgram program);

/*
 * Determines if the provided string is representative of an
 * option. An option must be prefixed with - or --, and be
 * immediately followed by an alphabetical character.
 *
 * @param name: the name to validate
 * @return: 1 if it is an option, 0 if it is not an option
*/
int libarg_is_option(const char *name);

/*
 * Performs error checking, and validates options and arguments.
 *
 * @param program: the program to validate
*/
void libarg_error(struct LibargProgram program);

/*
 * Retrieves the argument in the argv assigned to the template
 * argument in the argument array. If the argument is not provided,
 * and the argument is marked as default, the default is returned.
 * Otherwise, an error is raised.
 *
 * @param program: the program metadata
 * @param argument: the argument to retrieve
 * @return: the argv value
*/
char *libarg_get_argument(struct LibargProgram program, const char *argument);

/*
 * Determines the index of the first variadic argument in
 * the argv, or -1 if it is not provided.
 *
 * @param program: the program to search
 * @return: the index of the argument in the argv, or -1
*/
int libarg_find_variadic(struct LibargProgram program);

/*
 * Retrieves the index of the next variadic argument in
 * the argv, or -1 if it does not exist.
 *
 * @param program: the program to search
 * @param start: the index to start searching at
 * @return: the index of the next variadic argument, or -1
*/
int libarg_next_variadic(struct LibargProgram program, int start);

#endif
