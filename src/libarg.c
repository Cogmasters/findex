#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libarg.h"

/* START argument.c */

int libarg_count_arguments(struct LibargArgument *arguments) {
    int count = 0;
    size_t index = 0;

    if(arguments == NULL)
        return -1;

    for(index = 0; arguments[index].name != NULL; index++) {
        count++;
    }

    return count;
}

int libarg_argument_index(struct LibargArgument *arguments, const char *argument) {
    int index = 0;

    for(index = 0; arguments[index].name != NULL; index++) {
        if(strcmp(arguments[index].name, argument) != 0)
            continue;

        return index;
    }

    return -1;
}

char *libarg_get_argument(struct LibargProgram program, const char *argument) {
    int argv_index = 0;
    int argument_index = 0;
    int exists = libarg_argument_index(program.arguments, argument);
    int registered_arguments = libarg_count_arguments(program.arguments);

    /* This is not the correct function to get variadic arguments from */
    if(program.arguments[exists].type == LIBARG_VARIADIC) {
        fprintf(stderr, "libarg_get_argument: attempt to get variadic argument '%s'\n", program.arguments[exists].name);
        exit(EXIT_FAILURE);
    }

    for(argv_index = 1; argv_index < program.argc; argv_index++) {
        char *current_argument = program.argv[argv_index];

        if(argument_index == registered_arguments)
            break;

        /* Skip options */
        if(libarg_is_option(current_argument) == 1)  {
            argv_index += libarg_count_parameters(current_argument, program);

            continue;
        }

        if(strcmp(program.arguments[argument_index].name, argument) == 0) {
            return program.argv[argv_index];
        }

        argument_index++;
    }

    /* Provide a default value if it exists */
    if(exists != -1) {
        return program.arguments[exists].fallback;
    }

    fprintf(stderr, "libarg_get_argument: attempt to get unknown argument '%s'\n", argument);
    exit(EXIT_FAILURE);

    return NULL;
}

int libarg_find_variadic(struct LibargProgram program) {
    int argv_index = 0;
    int argument_index = 0;
    int registered_arguments = libarg_count_arguments(program.arguments);

    for(argv_index = 1; argv_index < program.argc; argv_index++) {
        char *current_argument = program.argv[argv_index];

        if(argument_index == registered_arguments)
            break;

        /* Skip options */
        if(libarg_is_option(current_argument) == 1)  {
            argv_index += libarg_count_parameters(current_argument, program);

            continue;
        }

        if(program.arguments[argument_index].type == LIBARG_VARIADIC) {
            return argv_index;
        }

        argument_index++;
    }

    return -1;
}

int libarg_next_variadic(struct LibargProgram program, int start) {
    int index = 0;

    for(index = start + 1; index < program.argc; index++) {
        char *argument = program.argv[index];

        /* Skip options */
        if(libarg_is_option(argument) == 1)  {
            index += libarg_count_parameters(argument, program);

            continue;
        }

        return index;
    }

    return -1;
}

/* END argument.c */

/* START error.c */

void libarg_error_validate_option(const char *option, struct LibargProgram program) {
    int arguments = 0;
    int argv_index = libarg_get_option(option, program);
    int option_index = libarg_find_option(option, program.options);

    if(option_index == -1) {
        fprintf(stderr, "%s: unrecognized option '%s'\n", program.name, option);
        fprintf(stderr, "Try '%s --help' for more information.\n", program.name);

        exit(EXIT_FAILURE);
    }

    /* Variadic options, or options that take no arguments need not be checked */
    switch(program.options[option_index].parameters) {
        case -1: return; /* Variadic */
        case 0 : return; /* Takes no arguments */
    }

    /* Count how many arguments this option has, since it has been shown
     * to not be invalid if it reaches this point. */
    while(argv_index + 1 < program.argc) {
        char *argument = program.argv[argv_index + 1];

        if(libarg_is_option(argument) == 1)
            break;

        arguments++;
        argv_index++;
    }

    if(arguments >= program.options[option_index].parameters)
        return;

    fprintf(stderr, "%s: option '%s' expected %i parameter(s), got %i\n", program.name, option, 
        program.options[option_index].parameters, arguments);
    fprintf(stderr, "Try '%s --help' for more information.\n", program.name);

    exit(EXIT_FAILURE);
}

int libarg_count_positionals(struct LibargProgram program) {
    int index = 0;
    int count = 0;

    for(index = 0; program.arguments[index].name != NULL; index++) {
        if(program.arguments[index].type != LIBARG_POSITIONAL)
            break;

        count++;
    }

    return count;
}

void libarg_error_options(struct LibargProgram program) {
    int index = 0;

    for(index = 0; index < program.argc; index++) {
        char *option = program.argv[index];

        if(libarg_is_option(option) == 0)
            continue;

        libarg_error_validate_option(option, program);
    }
}

void libarg_error_arguments(struct LibargProgram program) {
    int index = 0;
    int optionals = 0;
    int positionals = 0;
    int has_variadic = 0;
    int argument_index = 0;
    int gotten = 0;
    int expected = libarg_count_positionals(program);

    /* Count various number of arguments */
    for(index = 1; index < program.argc; index++) {
        char *argument = program.argv[index];
        
        /* Skip options */
        if(libarg_is_option(argument) == 1)  {
            index += libarg_count_parameters(argument, program);

            continue;
        }

        gotten++;

        if(program.arguments[argument_index].name == NULL)
            continue;

        /* Collect statistics about each type of argument */
        if(program.arguments[argument_index].type == LIBARG_POSITIONAL) {
            positionals++;
            argument_index++;

            continue;
        } else if(program.arguments[argument_index].type == LIBARG_OPTIONAL) {
            optionals++;
            argument_index++;

            continue;
        } else {
            has_variadic = 1;

            break;
        }

    }

    if(gotten < expected) {
        fprintf(stderr, "%s: too few arguments. expected %i, got %i\n", program.name, expected, gotten);
        fprintf(stderr, "Try '%s --help' for more information.\n", program.name);

        exit(EXIT_FAILURE);
    }

    /* If there are optional arguments, they should be subtracted from the total
     * assuming there are more than expected, because they are not *needed*. */
    if((gotten - optionals) > expected) {
        if(has_variadic == 1)
            return;

        fprintf(stderr, "%s: too many arguments. expected %i, got %i\n", program.name, expected, gotten);
        fprintf(stderr, "Try '%s --help' for more information.\n", program.name);

        exit(EXIT_FAILURE);
    }
}

void libarg_error(struct LibargProgram program) {
    libarg_error_options(program);
    libarg_error_arguments(program);
}

/* END error.c */

/* START option.c */

int libarg_count_options(struct LibargOption *options) {
    int count = 0;
    int index = 0;

    for(index = 0; options[index].longform != NULL; index++) {
        count++;
    }

    return count;
}

int libarg_find_option(const char *name, struct LibargOption *options) {
    int index = 0;

    if(options == NULL)
        return -1;

    for(index = 0; (options[index].longform != NULL) || (options[index].shortform != NULL); index++) {
        if((options[index].longform != NULL) && (strcmp(options[index].longform, name) == 0))
            return index;

        if((options[index].shortform != NULL) && (strcmp(options[index].shortform, name) == 0))
            return index;

        continue;
    }

    return -1;
}

int libarg_get_option(const char *name, struct LibargProgram program) {
    int index = 0;

    for(index = 0; index < program.argc; index++) {
        if(strcmp(program.argv[index], name) != 0)
            continue;

        return index;
    }

    return -1;
}

int libarg_is_option(const char *name) {
    int length = 0;

    if(name[0] == '-')
        length++;

    if(name[1] == '-')
        length++;

    if(length > 0 && ((name[length] >= 'A' && name[length] <= 'Z') || (name[length] >= 'a' && name[length] <= 'z')))
        return 1;

    return 0; 
}

int libarg_count_parameters(const char *name, struct LibargProgram program) {
    int argv_index = libarg_get_option(name, program);
    int array_index = libarg_find_option(name, program.options);
    int parameters = 0;

    if(array_index == -1) {
        fprintf(stderr, "libarg_count_parameters: attempt to find unregistered option '%s'\n", name);
        exit(EXIT_FAILURE);
    }

    /* This function's main purpose is to count variadic parameters, and
     * the validity of the number of option parameters should already be
     * tested. */
    if(program.options[array_index].parameters != -1)
        return program.options[array_index].parameters;

    while((argv_index + 1) < program.argc) {
        if(libarg_is_option(program.argv[argv_index + 1]) == 1)
            break;

        parameters++;
        argv_index++;
    }

    return parameters;
}

/* END option.c */
