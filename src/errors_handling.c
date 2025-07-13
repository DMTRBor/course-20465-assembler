#include "../hdr/errors_handling.h"


/* -------------------------- Pre-Assembler -------------------------- */

int is_macro_name_valid(char *line, int line_number) {
    int id;  /* array element index */

    /* check if command appears in macro name */
    for (id = 0; id < NUM_OF_COMMANDS; id++) {
        if (strstr(line, commands[id].name) != NULL) {
            fprintf(stderr,
                "Illegal argument in line %d: command '%s' found in line\n", line_number, commands[id].name);
            return FALSE;
        }
    }

    /* check if instruction appears in macro name */
    for (id = 0; id < NUM_OF_INSTRUCTIONS; id++) {
        if (strstr(line, instructions[id]) != NULL) {
            fprintf(stderr,
                "Illegal argument in line %d: instruction '%s' found in line\n", line_number, instructions[id]);
            return FALSE;
        }
    }

    /* check if register name appears in macro name */
    for (id = 0; id < NUM_OF_REGISTERS; id++) {
        if (strstr(line, registers[id]) != NULL) {
            fprintf(stderr,
                "Illegal argument in line %d: register '%s' found in line\n", line_number, registers[id]);
            return FALSE;
        }
    }

    return TRUE;
}


int is_macro_args_num_valid(char *line, int num_of_expected_args) {
    char *line_args;
    char *line_copy;
    int num_of_args = 0;  /* number of args in line */

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    while (line_args != NULL) {
        /* count argument */
        num_of_args++;
        /* next token/arg */
        line_args = strtok(NULL, WHITESPACE);
    }

    free(line_copy);
    return (num_of_args == num_of_expected_args);
}


/* -------------------------- First / Second Pass -------------------------- */

int is_command(char *arg) {
    int id;  /* array index */

    /* check if argument is a command */
    for (id = 0; id < NUM_OF_COMMANDS; id++) {
        if (strcmp(arg, commands[id].name) == STR_EQUAL) {
            return TRUE;
        }
    }

    return FALSE;  /* not a command */
}


int is_instruction(char *arg) {
    int id;  /* array index */

    /* check if argument is an instruction */
    for (id = 0; id < NUM_OF_INSTRUCTIONS; id++) {
        if (strcmp(arg, instructions[id]) == STR_EQUAL) {
            return TRUE;
        }
    }

    return FALSE;  /* not an instruction */
}


int is_valid_label(char *arg, int line_number) {
    int id;  /* array index */

    /* check if label starts with a letter */
    if (!isalpha(arg[0])) {
        fprintf(stderr,
            "Invalid label in line %d: '%s' must start with a letter\n", line_number, arg);
        return FALSE;
    }

    /* check if label contains only valid characters */
    for (id = 0; arg[id] != NULL_TERMINATOR; id++) {
        if (!isalnum(arg[id]) && arg[id] != UNDERSCORE) {
            fprintf(stderr,
                "Invalid label in line %d: '%s' contains invalid character '%c'\n", line_number, arg, arg[id]);
            return FALSE;
        }
    }

    return TRUE;  /* valid label */
}


/**
 * This function receives a line
 */
LineArg detect_and_validate_first_arg(char *line, int line_number) {
    char *line_args;
    char *line_copy;
    LineArg line_arg_type = ERROR;

    /* copy line for processing */
    line_copy = strdup(line);
    /* tokenize with whitespaces */
    line_args = strtok(line_copy, WHITESPACE);

    if (is_command(line_args)) {
        line_arg_type = COMMAND;
    }
    else if (is_instruction(line_args)) {
        line_arg_type = INSTRUCTION;
    }
    else if (is_valid_label(line_args, line_number)) {
        line_arg_type = LABEL;
    }
    else if (strcmp(line_args, NEWLINE_STR) == STR_EQUAL) {
        line_arg_type = EMPTY_LINE;
    }
    else if (line_args[0] == COMMENT_SIGN) {
        line_arg_type = COMMENT;
    }

    free(line_copy);
    return line_arg_type;
}